#pragma once
#include "../IPC.h"
#define IPCID_TestCommunication 0x01
#define IPCID_GetFunctionByIndex 0x02
#define IPCID_GetFunctionByName 0x03

void IpcTestCommunication(const struct IPCMessage_t* Message, struct IPCReply_t* Reply);

void IpcGetFunctionByIndex(const struct IPCMessage_t* Message, struct IPCReply_t* Reply);

void IpcGetFunctionByName(const struct IPCMessage_t* Message, struct IPCReply_t* Reply);