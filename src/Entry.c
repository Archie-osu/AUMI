#include <Windows.h>

void 
AUMI_Init()
{
	
}

int __stdcall 
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)(AUMI_Init), 0, 0, 0);

	return TRUE;
}