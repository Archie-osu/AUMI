#pragma once
#include "../Shared.h"

__declspec(dllexport) extern AUMIResult AUMI_CreateCode(CCode* outCode, void* CodeBuffer, int CodeBufferSize, int LocalVarsUsed, const char* Name);

__declspec(dllexport) extern AUMIResult AUMI_CreateYYCCode(CCode* outCode, PFUNC_YYGML Function, const char* FunctionName, const char* CodeName);

__declspec(dllexport) extern AUMIResult AUMI_FreeCode(CCode* Code);

__declspec(dllexport) extern AUMIResult AUMI_GetGlobalState(YYObjectBase** outState);

__declspec(dllexport) extern AUMIResult AUMI_ExecuteCode(YYObjectBase* Self, YYObjectBase* Other, CCode* Code, YYRValue* Arguments);

__declspec(dllexport) extern AUMIResult AUMI_GetCodeExecuteAddress(void** outAddress);

__declspec(dllexport) extern AUMIResult AUMI_GetCodeFunctionAddress(void** outAddress);

__declspec(dllexport) extern AUMIResult AUMI_GetFunctionByIndex(int index, struct AUMIFunctionInfo* outInformation);

__declspec(dllexport) extern AUMIResult AUMI_GetFunctionByName(const char* Name, struct AUMIFunctionInfo* outInformation);

__declspec(dllexport) extern AUMIResult AUMI_SetCompatibilityMode(bool NewState);

__declspec(dllexport) extern AUMIResult AUMI_CallBuiltinFunction(const char* Name, RValue* Result, YYObjectBase* Self, YYObjectBase* Other, int argc, RValue* Args);