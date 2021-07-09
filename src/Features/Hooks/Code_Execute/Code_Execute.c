#include "../../../Shared.h"
#include "../../Exports.h"
#include <stdio.h>

PFUNC_CEXEC opfnCode_Execute = NULL;

// AUMI hijacks code execution globally to allow for modification of scripts at runtime
char Hook_Code_Execute(YYObjectBase* selfinst, YYObjectBase* otherinst, CCode* code, RValue* res, int flags)
{
	// Ignore AUMI injections
	if (flags == AUMI_MAGIC)
		return opfnCode_Execute(selfinst, otherinst, code, res, 0);

	if (code->i_pVM)
	{
		// VM
	}

	else if (code->i_pFunc)
	{
		// YYC
	}

	return opfnCode_Execute(selfinst, otherinst, code, res, flags);
}
