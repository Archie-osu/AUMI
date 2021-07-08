#include "../IPC.h"
#include "../../../Shared.h"
#include "../../Exports.h"
#include <string.h>

void IpcTestCommunication(struct IPCMessage_t* Message, struct IPCReply_t* OutReply)
{
	OutReply->AUMIResult = AUMI_OK;
	strcpy_s(OutReply->Buffer, 128, "Hello from AUMI C - If you can read this, the IPC Test was successful!\n\nLet me just pad this reply out to 128 characters, done.");
}

void IpcGetFunctionByIndex(struct IPCMessage_t* Message, struct IPCReply_t* OutReply)
{
	struct AUMIFunctionInfo* RFInformation;

	AUMIResult result = AUMI_GetFunctionByIndex(*(int*)(Message->Buffer), &RFInformation);

	OutReply->AUMIResult = result;
	memcpy(OutReply->Buffer, &RFInformation, sizeof(struct AUMIFunctionInfo));
}

void IpcGetFunctionByName(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	struct AUMIFunctionInfo RFInformation;
	AUMIResult result = AUMI_GetFunctionByName(Message->Buffer, &RFInformation);

	Reply->AUMIResult = result;
	memcpy(Reply->Buffer, &RFInformation, sizeof(struct AUMIFunctionInfo));
}

void IpcExecuteCode(struct IPCMessage_t* Message, struct IPCReply_t* OutReply)
{
	const struct
	{
		int CodeSize;
		int LocalsUsed;
		char Code[512 - (sizeof(int) * 2)];
	} *CodeBuffer = Message->Buffer;

	CCode Code;
	AUMIResult result;
	RValue Arguments; memset(&Arguments, 0, sizeof(RValue));

	YYObjectBase* g_pGlobal = NULL;

	if (result = AUMI_GetGlobalState(&g_pGlobal))
	{
		OutReply->AUMIResult = result;
		return;
	}

	if (result = AUMI_CreateCode(&Code, CodeBuffer->Code, CodeBuffer->CodeSize, CodeBuffer->LocalsUsed, "AUMI-C Code Entry"))
	{
		OutReply->AUMIResult = result;
		return;
	}

	if (result = AUMI_ExecuteCode(g_pGlobal, g_pGlobal, &Code, &Arguments, 0))
	{
		OutReply->AUMIResult = result;
		AUMI_FreeCode(&Code);
		return;
	}

	AUMI_FreeCode(&Code);
	OutReply->AUMIResult = AUMI_OK;
}