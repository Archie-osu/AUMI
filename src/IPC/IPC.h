#pragma once
#include "../Shared.h"

//Used in commanding AUMI to do something, size: 512 bytes.
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

void IpcManagerWorker();