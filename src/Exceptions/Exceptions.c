#include <Windows.h>

// There might be single null-bytes inside the YYGMLException message, so read until you find at least two of them in a row, replacing the single ones with '-'.
static void
ReadYYExceptionMessage(const char* Message, char* Buffer, rsize_t Length)
{
	int index = 0;
	while (*(WORD*)&Message[index] != 0 && index <= Length)
	{
		if (Message[index] == '\x00')
		{
			Buffer[index] = '-';
			index++;
			continue;
		}

		Buffer[index] = Message[index];
		index++;
	}
}

// The pointer to the YYGMLException object is right after a null pointer, so loop until you find that.
static const char*
GetYYExceptionPointer(DWORD* ExceptionInformationEntry)
{
	while (*ExceptionInformationEntry != 0)
	{
		ExceptionInformationEntry++;
	}

	return *(const char**)(ExceptionInformationEntry + 1);
}

static BOOL
IsASCIIText(char ch)
{
	// The only exception
	if (ch == ' ')
		return TRUE;

	if (ch < 'A')
		return FALSE;

	if (ch > 'Z' && ch < 'a')
		return FALSE;

	if (ch > 'z')
		return FALSE;

	return TRUE;
}

// Increments the pointer until it encounters what it thinks is ASCII Text - used for getting rid of the "bonus info" inside the exception.
static const char*
FindASCIIText(const char* Pointer)
{
	while (!IsASCIIText(*Pointer) || !IsASCIIText(*(Pointer + 1))) // Check two characters to not get so many false positives.
	{
		Pointer++;
	}
	return Pointer;
}

int
ExceptionHandler(unsigned int Code, struct _EXCEPTION_POINTERS* ep)
{
	struct ThrowInfo
	{
		DWORD Attributes;
		DWORD pmfnUndwind;
		DWORD pForwardCompat;

		struct CatchableTypeArray
		{
			DWORD nCatchableTypes;
			struct CatchableType
			{
				unsigned int Reserved;
				struct TypeDescriptor
				{
					DWORD Hash;
					void* Spare;
					char ClassName[];
				} *TypeDescriptor;

			} *ArrayOfCatchableTypes;

		} *pCatchableTypeArray;
	};

	if (Code == 0xE06D7363) // Is this a C++ exception?
	{
		if (ep->ExceptionRecord->NumberParameters > 2)
		{
			struct ThrowInfo* firstLayer = ep->ExceptionRecord->ExceptionInformation[2];

			// Checking all these structs for null pointers proved way too ugly.
			if (strstr(firstLayer->pCatchableTypeArray->ArrayOfCatchableTypes->TypeDescriptor->ClassName, "YYGMLException"))
			{
				// It's a YYGMLException
				char Buffer[256] = { 0 };

				ReadYYExceptionMessage(FindASCIIText(GetYYExceptionPointer(ep->ExceptionRecord->ExceptionInformation[1])), Buffer, 256);

				MessageBoxA(0, Buffer, "AUMI - Code Error", MB_OK | MB_ICONERROR | MB_TOPMOST);
				exit(0);
			}
		}
	}

	return EXCEPTION_CONTINUE_SEARCH;
}
