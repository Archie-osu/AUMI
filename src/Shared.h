#pragma once

typedef enum __declspec(dllexport) AUMIResult
{
	AUMI_OK = 0,
	AUMI_NO_MEMORY = 1,
	AUMI_INVALID = 2,
	AUMI_NOT_FOUND = 3,
	AUMI_FAIL = 4,
	AUMI_NOT_IMPLEMENTED = 5,
} AUMIResult;