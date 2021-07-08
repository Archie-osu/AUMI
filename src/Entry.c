// AUMI-C
// My primary aim is to reconstruct important game API's, grouping them up in neat packages with (hopefully) clean code.

// I was THIS close to rewritting it in C++...
#define WIN32_LEAN_AND_MEAN 1
#include <Windows.h>
#include "Features/IPC/IPC.h"
#include "Features/Exports.h"
#include <stdio.h>

#if _DEBUG
#define UnitTests 1
#endif

void AcInitRoutine() // Handles initializing of AUMI
{
	// Debug-only testing of built-in functions
#if UnitTests
	AllocConsole();

	FILE* fDummy;
	freopen_s(&fDummy, "CONIN$", "r", stdin);
	freopen_s(&fDummy, "CONOUT$", "w", stderr);
	freopen_s(&fDummy, "CONOUT$", "w", stdout);

	AUMIResult Result;
	
	printf("Running tests...\n");

	// AUMI_GetFunctionByIndex
	{
		struct AUMIFunctionInfo Info; memset(&Info, 0, sizeof(struct AUMIFunctionInfo));

		Result = AUMI_GetFunctionByIndex(127, &Info);

		printf("Function index lookup: AUMIResult %i, Name %s, Routine 0x%p\n", Result, Info.Name, Info.Function);
	}

	//AUMI_GetFunctionByName
	{
		struct AUMIFunctionInfo Info; memset(&Info, 0, sizeof(struct AUMIFunctionInfo));

		Result = AUMI_GetFunctionByName("code_is_compiled", &Info);

		printf("Function name lookup: AUMIResult %i, Name %s, Routine 0x%p\n", Result, Info.Name, Info.Function);
	}

	//AUMI_GetGlobal
	{
		YYObjectBase* pGlobal = NULL;

		Result = AUMI_GetGlobalState(&pGlobal);

		printf("Get Global State: AUMIResult %i, Address: 0x%p\n", Result, pGlobal);
	}

	//AUMI_GetCodeExecuteAddress
	{
		void* pFunction = NULL;

		Result = AUMI_GetCodeExecuteAddress(&pFunction);

		printf("Code_Execute() Address: AUMIResult %i, Address 0x%p\n", Result, &pFunction);
	}
	
	//AUMI_GetCodeFunctionAddress
	{
		void* pFunction = NULL;

		Result = AUMI_GetCodeFunctionAddress(&pFunction);

		printf("code_function_GET_the_function() Address: AUMIResult %i, Address 0x%p\n", Result, &pFunction);
	}

	printf("Tests finished.\n");
#endif

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