#include <Windows.h>
#include "IPC/IPC.h"

int __stdcall 
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)IpcManagerWorker, 0, 0, 0);
	}
	return TRUE;
}