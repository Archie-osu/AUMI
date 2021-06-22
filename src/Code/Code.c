#define WIN32_LEAN_AND_MEAN
#include "Code.h"
#include "../Patterns/Patterns.h"
#include <string.h>
#include <stdlib.h>
#include <Windows.h>
#include <Psapi.h>
static long g_pGetFunctionFromArray = 0;

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
	return AUMI_NOT_IMPLEMENTED;
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

	char* pName = NULL;

	void(__cdecl* fn)(int, char**, void**, int*) = g_pGetFunctionFromArray;

	fn(inIndex, &pName, &refFunction->function, &refFunction->argnum);

	if (pName == NULL) {
		strcpy_s(refFunction->name, 64, "<Invalid function>");
		return AUMI_INVALID;
	}

	memcpy(refFunction->name, pName, 64);

	return AUMI_OK;
}

AUMIResult AiGetFunctionByName(const char* inName, struct RFunction* refFunction, int* outIndex)
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
			memcpy(refFunction, &rFunction, sizeof(struct RFunction));

			if (outIndex)
				*outIndex = Index;

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

	struct _Out
	{
		struct YYObjectBase* Value;
	};

	struct RFunction FunctionEntry;
	struct RValue Result;


	if (AiGetFunctionByName("@@GlobalScope@@", &FunctionEntry, NULL))
		return AUMI_NOT_FOUND;

	if (!FunctionEntry.function)
		return AUMI_FAIL;

	void(*fn)(struct RValue*, void*, void*, int, struct RValue*) = FunctionEntry.function;
	fn(&Result, NULL, NULL, 0, NULL);

	((struct _Out*)outInstance)->Value = Result.value;

	return AUMI_OK;
}
