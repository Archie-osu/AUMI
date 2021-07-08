#pragma once
typedef enum AUMIResult
{
	AUMI_OK = 0,				// The operation completed successfully.
	AUMI_FAIL = 1,				// Unspecified error occured, see source code.
	AUMI_UNAVAILABLE = 2,		// The called function is not available in the current context.
	AUMI_NO_MEMORY = 3,			// No more memory is available to the process.
	AUMI_NOT_FOUND = 4,			// The specified value could not be found.
	AUMI_NOT_IMPLEMENTED = 5,	// The specified function doesn't exist. (IPC error)
	AUMI_INVALID = 6			// One or more arguments were invalid.
} AUMIResult;

typedef void (*PFUNC_TROUTINE)(struct RValue* Result, struct YYObjectBase* Self, struct YYObjectBase* Other, int argc, struct RValue* Args);
typedef void (*PFUNC_YYGML)(struct YYObjectBase* Self, struct YYObjectBase* Other);
typedef char (__cdecl * PFUNC_CEXEC)(struct YYObjectBase* Self, struct YYObjectBase* Other, struct CCode* code, struct RValue* res, int flags);

#pragma pack(push, 4)
typedef struct YYRValue
{
	union
	{
		void* Pointer;
		double Value;
	};
	
	int Flags;
	int Kind;
} YYRValue, RValue;
#pragma pack(pop)

typedef struct RToken
{
	int kind;			// 0x4
	int type;			// 0x8
	int ind, ind2;		// 0xC, 0x10
	RValue value;		// 0x20
	int itemnumb;		// 0x24
	struct RToken* items; // 0x28
	int position;		// 0x2C
} RToken;

typedef struct VMBuffer
{
	void** vTable;
	int m_size;
	int m_numLocalVarsUsed;
	int m_numArguments;
	char* m_pBuffer;
	void** m_pConvertedBuffer;
	char* m_pJumpBuffer;
} VMBuffer;

#pragma pack(push, 4)
typedef struct CCode
{
	void** vTable;				// 0x4
	struct CCode* m_pNext;		// 0x8
	int i_kind;					// 0xC
	int i_compiled;				// 0x10
	const char* i_str;			// 0x14
	RToken i_token;				
	RValue i_value;
	VMBuffer* i_pVM;
	VMBuffer* i_pVMDebugInfo;
	char* i_pCode;
	const char* i_pName; //0x5C
	int i_CodeIndex;
	struct YYGMLFuncs* i_pFunc;
	char i_watch;
	int i_offset;
	int i_locals;
	int i_args;
	int i_flags;
	struct YYObjectBase* i_pPrototype;
} CCode;
#pragma pack(pop)

typedef struct YYObjectBase
{
	// CInstanceBase
	void** vTable;
	RValue* yyvars;

	// YYObjectBase

	struct YYObjectBase* m_pNextObject;
	struct YYObjectBase* m_pPrevObject;
	struct YYObjectBase* m_prototype;
	void* m_pcre;
	void* m_pcreExtra;
	const char* m_class;
	void(*m_GetOwnProperty)(struct YYObjectBase*, RValue*, const char*);
	void(*m_DeleteProperty)(struct YYObjectBase*, RValue*, const char*, char);
	void(*m_DefineOwnProperty)(struct YYObjectBase*, const char*, RValue*, char);
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
} YYObjectBase;

typedef struct __declspec(align(8)) YYVAR
{
	const char* pName;
	int val;
} YYVAR;

typedef struct YYGMLFuncs
{
	const char* pName;
	PFUNC_YYGML pFunc;
	YYVAR* pFuncVar;
} YYGMLFuncs;

struct AUMIFunctionInfo
{
	int Index;
	char Name[72];
	void* Function;
	int Arguments;
};

unsigned long FindPattern(const char* Pattern, const char* Mask, long base, unsigned size);