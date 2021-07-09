// AUMI-C
// My primary aim is to reconstruct important game API's, grouping them up in neat packages with (hopefully) clean code.

// I was THIS close to rewritting it in C++...
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "Features/IPC/IPC.h"
#include "Features/Exports.h"
#include "Features/Hooks/Hooks.h"
#include "MH/MinHook.h"

void AcInitRoutine() // Handles initializing of AUMI
{
	MH_Initialize();
	{
		void* CodeExecuteAddress = NULL;
		AUMI_GetCodeExecuteAddress(&CodeExecuteAddress);

		MH_CreateHook(GetYYErrorAddress(), Hook_YYError, &opfnYYError);
		MH_CreateHook(CodeExecuteAddress, Hook_Code_Execute, &opfnCode_Execute);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	while (1)
	{
		IPCManager();
		Sleep(5);
	}
}

int __stdcall
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)AcInitRoutine, 0, 0, 0);
	}
	return TRUE;
}