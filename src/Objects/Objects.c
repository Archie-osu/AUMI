#include "Objects.h"
#include <string.h>
#include <stdlib.h>
#include "../Patterns/Patterns.h"

//I use the variable_global_get function as a base for the scans.
AUMIResult 
ObGlobalInstance(void* outInstance, void* variable_global_get_fn, int Reserved)
{
	if (outInstance == NULL)
		return AUMI_INVALID;

	unsigned char* offset = PaFindAOB("\x8B\x00\x00\x00\x00\x00", "x?????", (long)variable_global_get_fn + Reserved, 64);

	if (!offset)
		return AUMI_NOT_FOUND;

	if (offset[1] >= 0x0D && offset[1] <= 0x3D) //Valid r32 mov opcode suffixes
	{
		*(void**)outInstance = *(void**)(offset + 2);
		return AUMI_OK;
		
	}

	//Avoid infinite loops
	if (Reserved > 32) 
		return AUMI_NOT_FOUND;

	return ObGlobalInstance(outInstance, variable_global_get_fn, Reserved + 1);
}

AUMIResult 
ObCreateCode(struct CCode* outCode, void* inVMCodeBuffer, int inBufferSize, int inLocalVarsUsed, const char* inName)
{
	memset(outCode, 0, sizeof(struct CCode));

	outCode->i_pVM = calloc(sizeof(struct VMBuffer_t), 1);
	if (!outCode->i_pVM)
		return AUMI_NO_MEMORY;

	outCode->i_pVM->m_pBuffer = calloc(inBufferSize, 1);
	if (!outCode->i_pVM->m_pBuffer)
		return AUMI_NO_MEMORY;

	outCode->i_compiled = 1;
	outCode->i_kind = 1;
	outCode->i_pName = inName;
	outCode->i_pVM->m_size = inBufferSize;
	outCode->i_pVM->m_numLocalVarsUsed = inLocalVarsUsed;
	memcpy(outCode->i_pVM->m_pBuffer, inVMCodeBuffer, inBufferSize);
	outCode->i_locals = inLocalVarsUsed;
	outCode->i_flags = 'AUMI'; //magic value

	return AUMI_OK;
}

AUMIResult 
ObDestroyCode(struct CCode* refCode)
{
	if (refCode->i_flags != 'AUMI') //not ours
		return AUMI_INVALID;

	free(refCode->i_pVM->m_pBuffer);
	free(refCode->i_pVM);
	return AUMI_OK;
}