#include "Wrappers.h"
#include "../../Code/Code.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>

void IpcGetFunctionByIndex(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	struct RFunction RFInformation;

	AUMIResult result = AiGetFunctionByIndex(*(int*)(Message->Buffer), &RFInformation);

	Reply->AUMIResult = result;
	memcpy(Reply->Buffer, &RFInformation, sizeof(struct RFunction));
}

void IpcGetFunctionByName(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	struct RFunction RFInformation;
	int outIndex;
	AUMIResult result = AiGetFunctionByName(Message->Buffer, &RFInformation, &outIndex);

	Reply->AUMIResult = result;
	memcpy(Reply->Buffer, &RFInformation, sizeof(struct RFunction));
	memcpy(Reply->Buffer + sizeof(struct RFunction), &outIndex, sizeof(int));
}

void IpcTestCommunication(const struct IPCMessage_t* Message, struct IPCReply_t* Reply)
{
	Reply->AUMIResult = AUMI_OK;
	strcpy_s(Reply->Buffer, 128, "Hello from AUMI - If you can read this, the IPC Test was successful!\nNow just making sure we copy all 128 characters. The end..");
}
