#pragma once
#include "../Shared.h"

//Used in commanding AUMI to do something, size: 64 bytes.
struct IPCMessage_t 
{
	short FuncID;
	char Buffer[62];
};

//AUMI replies sent to the server. 
//The FDReturn buffer contents depend on the function called.
//Interpreting the contents of the FDReturn buffer with an unsupported function may result in UB!
struct IPCReply_t 
{
	int AUMIResult; //Always contains a value.
	char Buffer[60]; //Buffer
};

void IpcManagerWorker();