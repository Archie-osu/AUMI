#define WIN32_LEAN_AND_MEAN
#include "Code.h"
#include "../Patterns/Patterns.h"
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <Psapi.h>
#include "../Exceptions/Exceptions.h"

static long g_pGetFunctionFromArray = 0;
static long g_pCodeExecute = 0;
static long g_pWADBaseAddress = 0; // Data file begin
static long g_pWADEndAddress = 0; // Data file end

static struct SLLVMVars* g_pLLVMVars = 0;

static MODULEINFO 
GetCurrentModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

AUMIResult 
AUMI_ExecuteCode(void* selfinst, void* otherinst, struct CCode* code, struct RValue* res, int flags)
{
	if (!code)
		return AUMI_INVALID;

	if (!g_pCodeExecute)
	{
		MODULEINFO CurrentModuleInfo = GetCurrentModuleInfo();
		g_pCodeExecute = PaFindAOB("\x8A\xD8\x83\xC4\x14\x80\xFB\x01\x74", "xxxxxxxxx", CurrentModuleInfo.lpBaseOfDll, CurrentModuleInfo.SizeOfImage);

		if (!g_pCodeExecute)
			return AUMI_NOT_FOUND;

		//Normalize the hook, wait for '0xCC, 0xCC' sequence
		while (*(WORD*)(g_pCodeExecute) != (WORD)0xCCCC)
			(char*)(g_pCodeExecute) -= 1;

		g_pCodeExecute += 2; //Compensate for the CC bytes
	}

	char(__cdecl * fn)(void*, void*, struct CCode*, struct RValue*, int) = g_pCodeExecute;

	int ret = 0;
	__try
	{
		ret = fn(selfinst, otherinst, code, res, flags);
	}
	__except (ExceptionHandler(GetExceptionCode(), GetExceptionInformation())) { /* What?! I'm just a comment that's catching bugs! */ }

	if (!ret)
		return AUMI_FAIL;

	return AUMI_OK;
}

AUMIResult 
AUMI_GetFunctionByIndex(int inIndex, struct RFunction* refFunction)
{
	if (!refFunction)
		return AUMI_INVALID;

	if (!g_pGetFunctionFromArray)
	{
		MODULEINFO CurrentModuleInfo = GetCurrentModuleInfo();
		g_pGetFunctionFromArray = PaFindAOB("\x8B\x44\x24\x04\x3B\x05\x00\x00\x00\x00\x7F", "xxxxxx????x", CurrentModuleInfo.lpBaseOfDll, CurrentModuleInfo.SizeOfImage);

		if (!g_pGetFunctionFromArray)
			return AUMI_NOT_FOUND;
	}

	void* pUnk1 = NULL;
	char* pName = NULL;

	void(__cdecl* fn)(int, char**, void**, int*, void*) = g_pGetFunctionFromArray;

	//The last param is missing from GMS2 runners, but __cdecl doesn't care about extra args, only cares about the missing ones :D
	fn(inIndex, &pName, &refFunction->function, &refFunction->argnum, &pUnk1);

	if (pName == NULL) {
		strcpy_s(refFunction->name, 64, "<Invalid function>");
		return AUMI_INVALID;
	}

	memcpy(refFunction->name, pName, 64);
	refFunction->index = inIndex;

	return AUMI_OK;
}

AUMIResult 
AUMI_GetFunctionByName(const char* inName, struct RFunction* refFunction)
{
	if (!refFunction)
		return AUMI_INVALID;

	int Index = 0;
	struct RFunction rFunction;

	while (1)
	{
		AUMIResult result = AUMI_GetFunctionByIndex(Index, &rFunction);
		if (result) 
			return AUMI_NOT_FOUND;
		
		if (!_stricmp(rFunction.name, inName)) 
		{
			refFunction->index = Index;
			memcpy(refFunction, &rFunction, sizeof(struct RFunction));

			return AUMI_OK;
		}
		
		Index++;
	}

	return AUMI_FAIL;
}

AUMIResult 
AUMI_GetGlobalInstance(void** outInstance)
{
	if (!outInstance)
		return AUMI_INVALID;

	struct RFunction FunctionEntry;
	struct RValue Result;


	if (AUMI_GetFunctionByName("@@GlobalScope@@", &FunctionEntry))
		return AUMI_NOT_FOUND;

	if (!FunctionEntry.function)
		return AUMI_FAIL;

	void(*fn)(struct RValue*, void*, void*, int, struct RValue*) = FunctionEntry.function;
	fn(&Result, NULL, NULL, 0, NULL);

	*outInstance = Result.value;

	return AUMI_OK;
}

AUMIResult 
AUMI_GetGameDataChunk(void** outChunk, const char* ID)
{
	if (!g_pWADBaseAddress || !g_pWADEndAddress)
	{
		// Create a dummy variable for the AUMI_GetGameData function.
		void* buf;
		AUMI_GetGameData(&buf); // Find the game data and populate the variables needed for this function.
	}

	struct RIFF* DataFile = g_pWADBaseAddress;
	char* CurrentChunk = &DataFile->Chunks;
	
	struct YYObjectBase* pGlobal = NULL;
	AUMI_GetGlobalInstance(&pGlobal);
	g_pLLVMVars->pGMLFuncs->pFunc(pGlobal, pGlobal);

	while (1)
	{
		// Are we out of bounds?
		if (CurrentChunk == NULL)
			return AUMI_NOT_FOUND;

		if (CurrentChunk > g_pWADEndAddress)
			return AUMI_NOT_FOUND;

		if (!memcmp(((struct RIFFChunk_t*)CurrentChunk)->ID, ID, 4))
		{
			*outChunk = CurrentChunk;
			return AUMI_OK;
		}

		// The ChunkSize is only the size of the Data field, thus we have to add 8 bytes to compensate for the ID and length.
		(char*)CurrentChunk += ((struct RIFFChunk_t*)CurrentChunk)->ChunkLength + 8;
	}

	return AUMI_FAIL; // How did we get here?
}

AUMIResult 
AUMI_GetGameData(void** outPointer)
{
	// This might cause some issues later, please find a better sig.
	if (!g_pWADBaseAddress || !g_pWADEndAddress)
	{
		MODULEINFO CurrentModuleInfo = GetCurrentModuleInfo();

		unsigned char* Signature = PaFindAOB(
			"\xE8\x00\x00\x00\x00" //call relative
			"\xE8\x00\x00\x00\x00" //second call relative
			"\xA1" //mov eax, ????
			, "x????x????x", CurrentModuleInfo.lpBaseOfDll, CurrentModuleInfo.SizeOfImage);
		

		if (*(Signature + 15) == 0xC7) // YYC Direct pointer version
			g_pWADBaseAddress = **(long**)(Signature + 17);
		else // VM push version
			g_pWADBaseAddress = **(long**)(Signature + 11);
			
		unsigned char* LLVMSignature = PaFindAOB("\x50\xE8\x00\x00\x00\x00\x83\xC4\x04", "xx????xxx", Signature, 48);

		LLVMSignature += 11;
		g_pLLVMVars = *(DWORD*)(LLVMSignature);

		// Is this the YYC double pointer variant?
		if (g_pLLVMVars->pWAD)
			if (memcmp(g_pLLVMVars->pWAD, "FORM", 4))
				g_pLLVMVars = *(struct SLLVMVars**)g_pLLVMVars;

		if (!g_pWADBaseAddress)
			return AUMI_NOT_FOUND;

		struct RIFF* Data = g_pWADBaseAddress;
		g_pWADEndAddress = ((char*)Data + Data->FileSize);
	}

	*outPointer = g_pWADBaseAddress;

	return AUMI_OK;
}