#include <Windows.h>
#include "IPC.h"

static HANDLE g_hPipeHandle = INVALID_HANDLE_VALUE;

//Windows-exclusive, sorry.
static int InitIPCPipe()
{
	if (g_hPipeHandle != INVALID_HANDLE_VALUE && g_hPipeHandle != 0)
		CloseHandle(g_hPipeHandle);

	g_hPipeHandle = CreateFileA(
		"\\\\.\\pipe\\AUMI-IPC",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL
	);

	if (g_hPipeHandle == INVALID_HANDLE_VALUE)
		return 0;

	return 1;
}

void IpcManagerWorker()
{
	MessageBoxA(0, "AUMI IPC online!", 0, 0);
	while (1)
	{
		if (!InitIPCPipe())
		{
			Sleep(5);
			continue;
		}

		struct IPCMessage_t MessageBuffer;
		DWORD dwBytesTransferred;

		BOOL result = ReadFile(g_hPipeHandle, &MessageBuffer, sizeof(MessageBuffer), &dwBytesTransferred, NULL);

		//Bail early if the file was read unsuccessful.
		if (!result)
		{
			Sleep(5);
			continue;
		}
			

		switch (MessageBuffer.FuncID)
		{
		case 0x01:
			MessageBoxA(0, "This is a test!", "AUMI - IPC Message", MB_OK);
			break;
		}

		Sleep(5);
	}
}