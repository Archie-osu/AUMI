#include "Code.h"
#include "../Patterns/Patterns.h"

static long g_pGetFunctionFromArray = 0;

AUMIResult CoExecuteCode(void* selfinst, void* otherinst, struct CCode* code, struct RValue* res, int flags)
{
	return AUMI_NOT_IMPLEMENTED;
}

AUMIResult GetFunctionByIndex(int inIndex, struct RFunction* refFunction)
{
	//This isn't done
	return AUMI_NOT_IMPLEMENTED;

	if (!refFunction)
		return AUMI_INVALID;

	if (!g_pGetFunctionFromArray)
	{
		g_pGetFunctionFromArray = PaFindAOB("\x8B\x44\x24\x04\x3B\x05\x00\x00\x00\x00\x7F", "xxxxxx????x", 0, 0);
	}
}

AUMIResult AiGetRawAddress(unsigned long* outAddress)
{
	return AUMI_NOT_IMPLEMENTED;
}
