#pragma once
#include "../Shared.h"

__declspec(dllexport) AUMIResult AiCreateCode(struct CCode* outCode, void* inVMCodeBuffer, int inBufferSize, int inLocalVarsUsed, const char* inName);

__declspec(dllexport) AUMIResult AiDestroyCode(struct CCode* refCode);