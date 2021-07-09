#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>
#include "../../../Shared.h"

PVOID opfnYYError = NULL;

// AUMI hijacks the GMS error handler, replacing it with one with no YYGMLExceptions
void Hook_YYError(const char* pFormat, ...)
{
	// Code has executed unsuccessfully!
	va_list vaArgs;
	va_start(vaArgs, pFormat);

	char Message[2048] = { 0 };
	strcpy_s(Message, 2048, pFormat);

	vsprintf_s(Message, 2048, pFormat, vaArgs);

	MessageBoxA(0, pFormat, "AUMI-C Code Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
}

void* GetYYErrorAddress()
{
	struct AUMIFunctionInfo mInfo;

	if (AUMI_GetFunctionByName("camera_create", &mInfo))
		return NULL;

	unsigned long Pattern = FindPattern("\x68\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x83\xC4", "x????x????xx", mInfo.Function, 64);

	unsigned long Relative = *(unsigned long*)(Pattern + 6);
	Relative = (Pattern + 10) + Relative; // eip = instruction base + 5 + relative offset

	return (void*)Relative;
}
