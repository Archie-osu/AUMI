#pragma once
#include "../Shared.h"
#define IPC_Test 0x01
#define IPC_FunctionAtIndex 0x02 //AiGetIndexFunc


//Used in commanding AUMI to do something, size: 512 bytes.
struct IPCMessage_t 
{
	short FuncID;
	char Buffer[510];
};

//AUMI replies sent to the server.
//Buffer contents change depending on the function called.
//The only thing guaranteed to have safe values is the AUMIResult member.
struct IPCReply_t 
{
	int AUMIResult; //Always contains a value.
	char Buffer[124]; //Buffer
};

void IpcManagerWorker();