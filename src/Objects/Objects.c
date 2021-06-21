#include "Objects.h"
#include <string.h>
#include <stdlib.h>
#include "../Patterns/Patterns.h"

AUMIResult 
AiGlobalInstance(void* outInstance)
{
	if (!outInstance)
		return AUMI_INVALID;

	//TODO: Call @@GlobalScope@@, result in RValue is the pointer.
	return AUMI_NOT_IMPLEMENTED;
}

AUMIResult 
AiCreateCode(struct CCode* outCode, void* inVMCodeBuffer, int inBufferSize, int inLocalVarsUsed, const char* inName)
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
AiDestroyCode(struct CCode* refCode)
{
	if (refCode->i_flags != 'AUMI') //not ours
		return AUMI_INVALID;

	free(refCode->i_pVM->m_pBuffer);
	free(refCode->i_pVM);
	return AUMI_OK;
}