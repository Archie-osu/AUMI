#pragma once
#include "../Shared.h"

struct __declspec(dllexport) RFunction
{
	char name[64];
	void* function;
	int argnum;
};


__declspec(dllexport) AUMIResult CoExecuteCode(void* selfinst, void* otherinst, struct CCode* code, struct RValue* res, int flags);

__declspec(dllexport) AUMIResult GetFunctionByIndex(int inIndex, struct RFunction* refFunction);

__declspec(dllexport) AUMIResult AiGetRawAddress(unsigned long* outAddress);