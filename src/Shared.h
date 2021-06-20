#pragma once
//Prefixes of functions go as follows:
//Ai prefix - Internal function, only usable with C / C++ code, not the IPC API.
//Ipc prefix - IPC-only functions, usually handlers that convert the message to Ai-function compatible arguments.

typedef enum __declspec(dllexport) AUMIResult
{
	AUMI_OK = 0,
	AUMI_NO_MEMORY = 1,
	AUMI_INVALID = 2,
	AUMI_NOT_FOUND = 3,
	AUMI_FAIL = 4,
	AUMI_NOT_IMPLEMENTED = 5,
} AUMIResult;