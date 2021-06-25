#define WIN32_LEAN_AND_MEAN
#include "Code.h"
#include "../Patterns/Patterns.h"
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <Psapi.h>
static long g_pGetFunctionFromArray = 0;
static long g_pCodeExecute = 0;

static MODULEINFO GetCurrentModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

AUMIResult AiExecuteCode(void* selfinst, void* otherinst, struct CCode* code, struct RValue* res, int flags)
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

	int ret = fn(selfinst, otherinst, code, res, flags);

	if (!ret)
		return AUMI_FAIL;

	return AUMI_OK;
}

AUMIResult AiGetFunctionByIndex(int inIndex, struct RFunction* refFunction)
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

AUMIResult AiGetFunctionByName(const char* inName, struct RFunction* refFunction)
{
	if (!refFunction)
		return AUMI_INVALID;

	int Index = 0;
	struct RFunction rFunction;

	while (1)
	{
		AUMIResult result = AiGetFunctionByIndex(Index, &rFunction);
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

AUMIResult AiGetGlobalInstance(void* outInstance)
{
	if (!outInstance)
		return AUMI_INVALID;

	struct RFunction FunctionEntry;
	struct RValue Result;


	if (AiGetFunctionByName("@@GlobalScope@@", &FunctionEntry))
		return AUMI_NOT_FOUND;

	if (!FunctionEntry.function)
		return AUMI_FAIL;

	void(*fn)(struct RValue*, void*, void*, int, struct RValue*) = FunctionEntry.function;
	fn(&Result, NULL, NULL, 0, NULL);

	*(void**)outInstance = Result.value;

	return AUMI_OK;
}
