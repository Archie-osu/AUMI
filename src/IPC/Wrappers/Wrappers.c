#include "Wrappers.h"
#include "../../Code/Code.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

static MODULEINFO GetCurrentModuleInfo()
{
	MODULEINFO modinfo = { 0 };
	HMODULE hModule = GetModuleHandleA(NULL);
	if (hModule == 0)
		return modinfo;
	GetModuleInformation(GetCurrentProcess(), hModule, &modinfo, sizeof(MODULEINFO));
	return modinfo;
}


void IpcGetFunctionByIndex(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	if (Message->FuncID != IPCID_GetFunctionByIndex)
	{
		Reply->AUMIResult = AUMI_INVALID;
		return;
	}
	
	struct RFunction RFInformation;
	HMODULE hMod = GetModuleHandleA(NULL);

	AUMIResult result = AiGetIndexFunc(*(int*)(Message->Buffer), &RFInformation, GetCurrentModuleInfo().lpBaseOfDll, GetCurrentModuleInfo().SizeOfImage);

	Reply->AUMIResult = result;
	memcpy(Reply->Buffer, &RFInformation, sizeof(struct RFunction));
}

void IpcTestCommunication(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	Reply->AUMIResult = AUMI_OK;
	strcpy_s(Reply->Buffer, 124, "Hello from AUMI - If you can read this, the IPC Test was successful!");
}
