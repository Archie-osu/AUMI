#pragma once
#include "../Shared.h"

__declspec(dllexport) AUMIResult AUMI_CreateCode(struct CCode* outCode, void* inVMCodeBuffer, int inBufferSize, int inLocalVarsUsed, const char* inName);

__declspec(dllexport) AUMIResult AUMI_DestroyCode(struct CCode* refCode);