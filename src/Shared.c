#include "Shared.h"
#include <stdlib.h>
#include <string.h>

AUMIResult YYRValue_CreateString(YYRValue* this, const char* String)
{
	if (!this)
		return AUMI_INVALID;

	this->pString = malloc(sizeof(RefString));

	if (!this->pString)
		return AUMI_NO_MEMORY;

	this->pString->m_Size = strlen(String) + 1;
	this->pString->m_refCount = 3; // Start with a high ref-count, so the game doesn't try to delete it and crash.
	this->Kind = 1; // KIND_STRING

	this->pString->m_String = malloc(this->pString->m_Size + 1);

	if (!this->pString->m_String)
	{
		free(this->pString);
		return AUMI_NO_MEMORY;
	}

	memset(this->pString->m_String, 0, this->pString->m_Size + 1); // Properly null-terminate the string
	strcpy_s(this->pString->m_String, this->pString->m_Size, String);

	return AUMI_OK;
}

AUMIResult YYRValue_FreeString(YYRValue* this)
{
	if (!this)
		return AUMI_INVALID;

	if (!this->pString)
		return AUMI_INVALID;

	if (this->Kind != 1)
		return AUMI_INVALID;

	if (this->pString->m_refCount != 3)
		return AUMI_INVALID;

	if (this->pString->m_String)
		free(this->pString->m_String);

	free(this->pString);

	return AUMI_OK;
}

unsigned long FindPattern(const char* Pattern, const char* Mask, long base, unsigned size)
{
	size_t PatternSize = strlen(Mask);

	for (unsigned i = 0; i < size - PatternSize; i++)
	{
		int found = 1;
		for (unsigned j = 0; j < PatternSize; j++)
		{
			found &= Mask[j] == '?' || Pattern[j] == *(char*)(base + i + j);
		}

		if (found)
			return (base + i);
	}

	return (unsigned long)NULL;
}