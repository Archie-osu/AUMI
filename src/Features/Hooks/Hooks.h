#pragma once

// Variables 
extern char(__cdecl* opfnCode_Execute)(struct YYObjectBase*, struct YYObjectBase*, struct CCode*, struct RValue*, int);

extern void* opfnYYError;

// Functions
extern void Hook_YYError(const char* pFormat, ...);

extern void* GetYYErrorAddress();

extern char Hook_Code_Execute(struct YYObjectBase*, struct YYObjectBase*, struct CCode*, struct RValue*, int);
