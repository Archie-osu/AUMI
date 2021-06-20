#include "Code.h"
#include "../Patterns/Patterns.h"
#include <string.h>

static long g_pGetFunctionFromArray = 0;

AUMIResult AiExecuteCode(void* selfinst, void* otherinst, struct CCode* code, struct RValue* res, int flags)
{
	return AUMI_NOT_IMPLEMENTED;
}

AUMIResult AiGetIndexFunc(int inIndex, struct RFunction* refFunction, unsigned long inBase, unsigned inSize)
{
	if (!refFunction)
		return AUMI_INVALID;

	if (!g_pGetFunctionFromArray)
	{
		g_pGetFunctionFromArray = PaFindAOB("\x8B\x44\x24\x04\x3B\x05\x00\x00\x00\x00\x7F", "xxxxxx????x", inBase, inSize);

		if (!g_pGetFunctionFromArray)
			return AUMI_NOT_FOUND;
	}

	char* pName = NULL;

	void(__cdecl* fn)(int, char**, void**, int*) = g_pGetFunctionFromArray;

	fn(inIndex, &pName, &refFunction->function, &refFunction->argnum);

	if (pName == NULL) {
		strcpy_s(refFunction->name, 64, "<Invalid function>");
		return AUMI_INVALID;
	}
		

	strcpy_s(refFunction->name, 64, pName);

	return AUMI_OK;
}