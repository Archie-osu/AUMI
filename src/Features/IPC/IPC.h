#pragma once
//Used in commanding AUMI to do something.
struct IPCMessage_t
{
	short FuncID;
	char Buffer[512];
};

//AUMI replies sent to the server.
//Buffer contents change depending on the function called.
//The only thing guaranteed to have safe values is the AUMIResult member.
struct IPCReply_t
{
	int AUMIResult; //Always contains a value.
	char Buffer[128]; //Buffer
};

void IPCManager();

extern void IpcTestCommunication(struct IPCMessage_t* Message, struct IPCReply_t* OutReply);

extern void IpcGetFunctionByIndex(struct IPCMessage_t* Message, struct IPCReply_t* OutReply);

extern void IpcGetFunctionByName(struct IPCMessage_t* Message, struct IPCReply_t* OutReply);

extern void IpcExecuteCode(struct IPCMessage_t* Message, struct IPCReply_t* OutReply);