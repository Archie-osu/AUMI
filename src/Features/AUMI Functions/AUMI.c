#include "../../Shared.h"
#include "../Exports.h"
#include <string.h>
#include <stdlib.h>

#include <Windows.h>
#include <Psapi.h>
#include "../../MH/MinHook.h"

#include "../../HDE/hde32.h"

static PFUNC_CEXEC g_pCodeExecute = NULL;
static void(*g_pGrabCodeFunction)(int, char** Name, void** Routine, int* Argc, void**) = NULL;

static MODULEINFO GetCurrentModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}

AUMIResult AUMI_CreateCode(CCode* outCode, void* CodeBuffer, int CodeBufferSize, int LocalVarsUsed, const char* Name)
{
	if (!outCode)
		return AUMI_INVALID;

	memset(outCode, 0, sizeof(CCode));

	outCode->i_pVM = calloc(sizeof(VMBuffer), 1);
	if (!outCode->i_pVM)
		return AUMI_NO_MEMORY;

	outCode->i_pVM->m_pBuffer = calloc(CodeBufferSize, 1);
	if (!outCode->i_pVM->m_pBuffer)
		return AUMI_NO_MEMORY;

	outCode->i_compiled = 1;
	outCode->i_kind = 1;
	outCode->i_pName = Name;
	outCode->i_pVM->m_size = CodeBufferSize;
	outCode->i_pVM->m_numLocalVarsUsed = LocalVarsUsed;
	memcpy(outCode->i_pVM->m_pBuffer, CodeBuffer, CodeBufferSize);
	outCode->i_locals = LocalVarsUsed;
	outCode->i_flags = AUMI_MAGIC; //magic value

	return AUMI_OK;
}

AUMIResult AUMI_CreateYYCCode(CCode* outCode, PFUNC_YYGML Function, const char* FunctionName, const char* CodeName)
{
	if (!outCode)
		return AUMI_INVALID;

	memset(outCode, 0, sizeof(CCode));

	outCode->i_compiled = 1;
	outCode->i_kind = 1;
	outCode->i_pName = CodeName;
	outCode->i_pFunc = malloc(sizeof(YYGMLFuncs));

	if (!outCode->i_pFunc)
		return AUMI_NO_MEMORY;

	outCode->i_pFunc->pFunc = Function;
	outCode->i_pFunc->pName = FunctionName;

	outCode->i_flags = 'AUMI'; //magic value

	return AUMI_OK;
}

AUMIResult AUMI_FreeCode(CCode* Code)
{
	if (!Code)
		return AUMI_INVALID;

	if (Code->i_flags != AUMI_MAGIC) //not ours
		return AUMI_INVALID;

	if (Code->i_pVM)
	{
		if (Code->i_pVM->m_pBuffer)
			free(Code->i_pVM->m_pBuffer);

		free(Code->i_pVM);
	}

	if (Code->i_pFunc)
		free(Code->i_pFunc);

	return AUMI_OK;
}

AUMIResult AUMI_GetGlobalState(YYObjectBase** outState)
{
	if (!outState)
		return AUMI_INVALID;

	struct AUMIFunctionInfo FunctionEntry;
	RValue Result;

	if (AUMI_GetFunctionByName("@@GlobalScope@@", &FunctionEntry))
		return AUMI_NOT_FOUND;

	if (!FunctionEntry.Function)
		return AUMI_NOT_FOUND;

	((PFUNC_TROUTINE)FunctionEntry.Function)(&Result, NULL, NULL, NULL, NULL);

	*outState = Result.Pointer;

	return AUMI_OK;
}

AUMIResult AUMI_ExecuteCode(YYObjectBase* Self, YYObjectBase* Other, CCode* Code, YYRValue* Arguments)
{
	if (!Code)
		return AUMI_INVALID;

	if (!g_pCodeExecute)
	{
		AUMIResult Result;
		if (Result = AUMI_GetCodeExecuteAddress(&g_pCodeExecute))
			return Result;
	}

	int ret = 0;
	ret = g_pCodeExecute(Self, Other, Code, Arguments, 0);

	return (ret == 1) ? AUMI_OK : AUMI_FAIL;
}

AUMIResult AUMI_GetCodeExecuteAddress(void** outAddress)
{
	if (!outAddress)
		return AUMI_INVALID;

	AUMI_SetCompatibilityMode(true); // Prevent incompatibilities with hooked routines

	MODULEINFO CurInfo = GetCurrentModuleInfo();
	void* Base = FindPattern("\x8A\xD8\x83\xC4\x14\x80\xFB\x01\x74", "xxxxxxxxx", CurInfo.lpBaseOfDll, CurInfo.SizeOfImage);

	if (!Base)
	{
		AUMI_SetCompatibilityMode(false);
		return AUMI_NOT_FOUND;
	}
		

	while (*(WORD*)Base != 0xCCCC)
		(char*)Base -= 1;
	
	(char*)Base += 2; // Compensate for the extra CC bytes

	*outAddress = Base;

	AUMI_SetCompatibilityMode(false);

	return AUMI_OK;
}

AUMIResult AUMI_GetCodeFunctionAddress(void** outAddress)
{
	MODULEINFO CurInfo = GetCurrentModuleInfo();

	AUMI_SetCompatibilityMode(true); // Prevent incompatibilities with hooked routines

	if (!(*outAddress = FindPattern("\x8B\x44\x24\x04\x3B\x05\x00\x00\x00\x00\x7F", "xxxxxx????x", CurInfo.lpBaseOfDll, CurInfo.SizeOfImage)))
	{
		AUMI_SetCompatibilityMode(false);
		return AUMI_NOT_FOUND;
	}
	
	AUMI_SetCompatibilityMode(false);
	return AUMI_OK;
}

AUMIResult AUMI_GetFunctionByIndex(int index, struct AUMIFunctionInfo* outInformation)
{
	if (!outInformation)
		return AUMI_INVALID;

	memset(outInformation, 0, sizeof(struct AUMIFunctionInfo));

	if (!g_pGrabCodeFunction)
	{
		AUMIResult Result;
		if (Result = AUMI_GetCodeFunctionAddress(&g_pGrabCodeFunction))
			return Result;
	}

	
	int Argc = 'AUMI'; //If this doesn't change, the function index is invalid.

	{
		void* pLastArg = NULL; // Made for compatibility with GMS 1

		g_pGrabCodeFunction(index, &outInformation->Name, &outInformation->Function, &Argc, &pLastArg);

		memcpy(outInformation->Name, *(void**)outInformation->Name, 64); // This apparently fixes strings, don't ask.
	}
	

	if (Argc == 'AUMI')
		return AUMI_NOT_FOUND;

	outInformation->Arguments = Argc;
	outInformation->Index = index;

	return AUMI_OK;
}

AUMIResult AUMI_GetFunctionByName(const char* Name, struct AUMIFunctionInfo* outInformation)
{
	if (!outInformation)
		return AUMI_INVALID;

	int Index = 0;
	struct AUMIFunctionInfo Info;

	while (1)
	{
		AUMIResult result;
		if (result = AUMI_GetFunctionByIndex(Index, &Info))
			return result;

		if (!_stricmp(Info.Name, Name))
		{
			outInformation->Index = Index;
			memcpy(outInformation, &Info, sizeof(struct AUMIFunctionInfo));

			return AUMI_OK;
		}

		Index++;
	}

	return AUMI_FAIL; // How did we get here?
}

AUMIResult AUMI_SetCompatibilityMode(bool NewState)
{
	if (NewState)
		return (MH_DisableHook(MH_ALL_HOOKS) == MH_OK) ? AUMI_OK : AUMI_FAIL;
	
	return (MH_EnableHook(MH_ALL_HOOKS) == MH_OK) ? AUMI_OK : AUMI_FAIL;
}

AUMIResult AUMI_CallBuiltinFunction(const char* Name, RValue* Result, YYObjectBase* Self, YYObjectBase* Other, int argc, RValue* Args)
{
	struct AUMIFunctionInfo mInfo;
	AUMIResult result;

	if (result = AUMI_GetFunctionByName(Name, &mInfo))
		return result;

	mInfo.Function(Result, Self, Other, argc, Args);

	return AUMI_OK;
}
