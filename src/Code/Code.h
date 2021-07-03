#pragma once
#include "../Shared.h"

struct __declspec(dllexport) RFunction
{
	int index;
	char name[64];
	void* function;
	int argnum;
};


__declspec(dllexport) AUMIResult AUMI_ExecuteCode(void* selfinst, void* otherinst, struct CCode* code, struct RValue* res, int flags);

__declspec(dllexport) AUMIResult AUMI_GetFunctionByIndex(int inIndex, struct RFunction* refFunction);

__declspec(dllexport) AUMIResult AUMI_GetFunctionByName(const char* inName, struct RFunction* refFunction);

__declspec(dllexport) AUMIResult AUMI_GetGlobalInstance(void** outInstance);

__declspec(dllexport) AUMIResult AUMI_GetGameData(void** outPointer);

__declspec(dllexport) AUMIResult AUMI_GetGameDataChunk(void** outPointer, const char* ID);