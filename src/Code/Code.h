#pragma once
#include "../Shared.h"

struct __declspec(dllexport) RFunction
{
	char name[64];
	void* function;
	int argnum;
};


__declspec(dllexport) AUMIResult AiExecuteCode(void* selfinst, void* otherinst, struct CCode* code, struct RValue* res, int flags);

__declspec(dllexport) AUMIResult AiGetIndexFunc(int inIndex, struct RFunction* refFunction, unsigned long inBase, unsigned inSize);