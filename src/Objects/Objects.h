#pragma once
#include "../Shared.h"

struct RValue
{
	void* value;
	void* secondvalue;

	int flags;
	int kind;
};

struct RToken
{
	int kind;
	int type;
	int ind, ind2;
	struct RValue value;
	int itemnumb;
	struct RToken* items;
	int position;
};

struct VMBuffer_t
{
	void** vTable;
	int m_size;
	int m_numLocalVarsUsed;
	int m_numArguments;
	char* m_pBuffer;
	void** m_pConvertedBuffer;
	char* m_pJumpBuffer;
};

#pragma pack(push, 4)
struct CCode
{
	void** vTable;
	struct CCode* m_pNext;
	int i_kind;
	int i_compiled;
	const char* i_str;
	struct RToken i_token;
	struct RValue i_value;
	struct VMBuffer_t* i_pVM;
	struct VMBuffer_t* i_pVMDebugInfo;
	char* i_pCode;
	const char* i_pName; //0x5C
	int i_CodeIndex;
	void* i_pFunc;
	char i_watch;
	int i_offset;
	int i_locals;
	int i_args;
	int i_flags;
	void* i_pPrototype;

};
#pragma pack(pop)

__declspec(dllexport) AUMIResult AiGlobalInstance(void* outInstance, void* inVariableGlobalGetFN, int Reserved);

__declspec(dllexport) AUMIResult AiCreateCode(struct CCode* outCode, void* inVMCodeBuffer, int inBufferSize, int inLocalVarsUsed, const char* inName);

__declspec(dllexport) AUMIResult AiDestroyCode(struct CCode* refCode);