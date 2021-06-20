#pragma once
#include "../IPC.h"
#define IPCID_TestCommunication 0x01
#define IPCID_GetFunctionByIndex 0x02

void IpcGetFunctionByIndex(const struct IPCMessage_t* Message, struct IPCReply_t* Reply);

void IpcTestCommunication(const struct IPCMessage_t* Message, struct IPCReply_t* Reply);