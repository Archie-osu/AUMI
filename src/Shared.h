#pragma once
//Prefixes of functions go as follows:
//Ai prefix - Internal function, only usable with C / C++ code, not the IPC API.
//Ipc prefix - IPC-only functions, usually handlers that convert the message to Ai-function compatible arguments.

typedef enum __declspec(dllexport) AUMIResult
{
	AUMI_OK = 0,
	AUMI_NO_MEMORY = 1,
	AUMI_INVALID = 2,
	AUMI_NOT_FOUND = 3,
	AUMI_FAIL = 4,
	AUMI_NOT_IMPLEMENTED = 5,
} AUMIResult;

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

struct RIFFChunk_t
{
	char ID[4];
	unsigned int ChunkLength;
	char* Data;
};

struct RIFF
{
	char ID[4];
	unsigned int FileSize;
	char* Chunks;
};

struct YYObjectBase
{
	// CInstanceBase
	void** vTable;
	struct RValue* yyvars;

	// YYObjectBase

	struct YYObjectBase* m_pNextObject;
	struct YYObjectBase* m_pPrevObject;
	struct YYObjectBase* m_prototype;
	void* m_pcre;
	void* m_pcreExtra;
	const char* m_class;
	void(*m_GetOwnProperty)(struct YYObjectBase*, struct RValue*, const char*);
	void(*m_DeleteProperty)(struct YYObjectBase*, struct RValue*, const char*, char);
	void(*m_DefineOwnProperty)(struct YYObjectBase*, const char*, struct RValue*, char);
	void* m_yyvarsMap;
	void** m_pWeakRefs;
	unsigned int m_numWeakRefs;
	unsigned int m_nvars;
	unsigned int m_flags;
	unsigned int m_capacity;
	unsigned int m_visited;
	unsigned int m_visitedGC;
	int m_GCgen;
	int m_GCcreationframe;
	int m_slot;
	int m_kind;
	int m_rvalueInitType;
	int m_curSlot;
};

struct __declspec(align(8)) YYVAR
{
	const char* pName;
	int Value;
};

typedef void (*PFUNC_YYGML)(struct CInstance*, struct CInstance*);

struct YYGMLFuncs
{
	const char* pName;
	PFUNC_YYGML pFunc;
	struct YYVAR* pFuncVar;
};

struct SLLVMVars
{
	char* pWAD; // Optional, base address of data file in memory
	int nWADFileLength; // Length of the data file
	int nGlobalVariables;
	int nInstanceVariables;
	int nYYCode;
	struct YYVAR** ppVars; //haha pp
	struct YYVAR** ppFuncs;
	struct YYGMLFuncs* pGMLFuncs;
	void* pYYStackTrace;
};