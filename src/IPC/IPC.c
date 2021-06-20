#include <Windows.h>
#include "Wrappers/Wrappers.h"

static HANDLE g_hPipeHandle = INVALID_HANDLE_VALUE;

//Windows-exclusive, sorry.
static BOOL InitIPCPipe()
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
		return FALSE;

	return TRUE;
}

static BOOL IpcPostReply(struct IPCReply_t* pReply)
{
	DWORD dwBytesTransferred;
	return WriteFile(g_hPipeHandle, pReply, sizeof(struct IPCReply_t), &dwBytesTransferred, NULL);
}

void IpcManagerWorker()
{
	while (1)
	{
		if (InitIPCPipe())
		{
			struct IPCMessage_t MessageBuffer;
			struct IPCReply_t MessageReply;
			DWORD dwBytesTransferred;

			BOOL result = ReadFile(g_hPipeHandle, &MessageBuffer, sizeof(MessageBuffer), &dwBytesTransferred, NULL);
			if (result)
			{
				switch (MessageBuffer.FuncID)
				{
				case IPCID_TestCommunication:
					IpcTestCommunication(&MessageBuffer, &MessageReply);
					IpcPostReply(&MessageReply);
					break;
				case IPCID_GetFunctionByIndex:
					IpcGetFunctionByIndex(&MessageBuffer, &MessageReply);
					IpcPostReply(&MessageReply);
					break;
				default:
					MessageReply.AUMIResult = AUMI_NOT_IMPLEMENTED;
					IpcPostReply(&MessageReply);
				}
			}
		}
		Sleep(5);
	}
}