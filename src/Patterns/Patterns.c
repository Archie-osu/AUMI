#pragma once
#include "Patterns.h"
#include <string.h>

unsigned long PaFindAOB(const char* Pattern, const char* Mask, long base, unsigned size)
{
	size_t PatternSize = strlen(Mask);

	for (unsigned i = 0; i < size - PatternSize; i++)
	{
		char found = 0;
		for (unsigned j = 0; j < PatternSize; j++)
		{
			found &= Mask[j] == '?' || Pattern[j] == *(char*)(base + i + j);
		}

		if (found)
			return (base + i);
	}

	return (unsigned long)NULL;
}
