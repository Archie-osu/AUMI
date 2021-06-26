#include "Wrappers.h"

#include "../../Code/Code.h"
#include "../../Objects/Objects.h"

#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

void 
IpcGetFunctionByIndex(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	struct RFunction RFInformation;

	AUMIResult result = AUMI_GetFunctionByIndex(*(int*)(Message->Buffer), &RFInformation);

	Reply->AUMIResult = result;
	memcpy(Reply->Buffer, &RFInformation, sizeof(struct RFunction));
}

void 
IpcGetFunctionByName(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	struct RFunction RFInformation;
	AUMIResult result = AUMI_GetFunctionByName(Message->Buffer, &RFInformation);

	Reply->AUMIResult = result;
	memcpy(Reply->Buffer, &RFInformation, sizeof(struct RFunction));
}

void 
IpcExecuteCode(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	const struct
	{
		int CodeSize;
		int LocalsUsed;
		char Code[512 - (sizeof(int) * 2)];
	} *CodeBuffer = Message->Buffer;

	struct CCode Code;
	AUMIResult result;
	void* g_pGlobal = NULL;
	struct RValue rv; memset(&rv, 0, sizeof(struct RValue));

	if (result = AUMI_GetGlobalInstance(&g_pGlobal))
	{
		Reply->AUMIResult = result;
		return;
	}

	if (result = AUMI_CreateCode(&Code, CodeBuffer->Code, CodeBuffer->CodeSize, CodeBuffer->LocalsUsed, "AUMI Code Entry"))
	{
		Reply->AUMIResult = result;
		return;
	}

	if (result = AUMI_ExecuteCode(g_pGlobal, g_pGlobal, &Code, &rv, 0))
	{
		Reply->AUMIResult = result;
		AUMI_DestroyCode(&Code);
		return;
	}

	AUMI_DestroyCode(&Code); // Memory leak what?
	Reply->AUMIResult = AUMI_OK;
}

void 
IpcTestCommunication(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	Reply->AUMIResult = AUMI_OK;
	strcpy_s(Reply->Buffer, 128, "Hello from AUMI - If you can read this, the IPC Test was successful!\nNow just making sure we copy all 128 characters. The end..");
}
