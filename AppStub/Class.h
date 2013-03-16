// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <Windows.h>
#include <string>
#include <vector>
#include <list>

#include "AppStub.h"

// --------------------------------------------------------------------------------
//  These structures need to stay in-sync with stuff in compiler!
// --------------------------------------------------------------------------------
enum VariableType
{
	VT_GLOBAL = 0,
	VT_LOCAL,
	VT_CONST,
	VT_PARAMETER,
	VT_FIELD,
};
enum PrimitiveDataType
{
	PDT_BOOL = 0,
	PDT_BYTE,
	PDT_SHORT,
	PDT_INT,
	PDT_LONG,
	PDT_FLOAT,
	PDT_DOUBLE,
	PDT_STRING,
	PDT_VOID,
	PDT_CLASS
};
enum AccessModifierLevel
{
	AM_PUBLIC,
	AM_PRIVATE,
	AM_PROTECTED
};

// --------------------------------------------------------------------------------
//  Metadata class definitions.
// --------------------------------------------------------------------------------

#define META_TYPE_ANY			-1
#define META_TYPE_VARIABLE		1
#define META_TYPE_FUNCTION		2
#define META_TYPE_CLASS			3
#define META_TYPE_NAMESPACE		4

struct MetaDataDefinition;
struct ClassDefinition;
struct VariableDefinition;
struct FunctionDefinition;

// Base meta-data definition for variables/functions/etc.
struct MetaDataDefinition
{
	// General information.
	int symbolType;

	// Declaration in source file.
	char* ptrToDeclaredFileName;
	int declaredFileLine;
	int declaredFileOffset;
	
	// General properties.
	char* ptrToName;
	
	// Child information.
	MetaDataDefinition** childList;
};

// Stores meta information about a namespace.
struct NamespaceDefinition
{
	// -----------------------------------------------------------
	
	// General information.
	int symbolType;

	// Declaration in source file.
	char* ptrToDeclaredFileName;
	int declaredFileLine;
	int declaredFileOffset;
	
	// Class properties.
	char* ptrToName;

	// Child information.
	MetaDataDefinition** childList;

	// -----------------------------------------------------------
};

// Stores meta information about a class, used
// to allocate and deallocate classes.
struct ClassDefinition
{
	// -----------------------------------------------------------
	
	// General information.
	int symbolType;

	// Declaration in source file.
	char* ptrToDeclaredFileName;
	int declaredFileLine;
	int declaredFileOffset;
	
	// Class properties.
	char* ptrToName;

	// Child information.
	MetaDataDefinition** childList;

	// -----------------------------------------------------------

	// Keep this at the top - prevents padding and because
	// compiler assumes this is first.
	int* ptrToVFTable;
	int vfTableSize;

	// Class hierarchy.
	int memoryDataSize;
	int superClassIndex;
	int classIndex;

	// Build in methods.
	void (*internalConstructMethod)(int);//int internalConstructMethod;//
	void (*internalDisposeMethod)(int);//int internalDisposeMethod;//
};

// Stores meta information about a variable thats
// been declared.
struct VariableDefinition
{
	// -----------------------------------------------------------
	
	// General information.
	int symbolType;

	// Declaration in source file.
	char* ptrToDeclaredFileName;
	int declaredFileLine;
	int declaredFileOffset;
	
	// General properties.
	char* ptrToName;
	
	// Child information.
	MetaDataDefinition** childList;

	// -----------------------------------------------------------

	// Variable specific.
	void* variableAddress;
	VariableType variableType;
	PrimitiveDataType dataType;
	int dataTypeClassDefinitionIndex;
	int stackOffset;
	int paramStackOffset;
	int classOffset;
	bool isConst;
	bool isBase;
	AccessModifierLevel accessModifier;	
};

// Stores meta information about a function thats
// been declared.
struct FunctionDefinition
{
	// -----------------------------------------------------------
	
	// General information.
	int symbolType;

	// Declaration in source file.
	char* ptrToDeclaredFileName;
	int declaredFileLine;
	int declaredFileOffset;
	
	// General properties.
	char* ptrToName;
	
	// Child information.
	MetaDataDefinition** childList;

	// -----------------------------------------------------------

	// Function specific.
	int* functionAddress;
	PrimitiveDataType returnDataType;
	int returnDataTypeClassDefinitionIndex;
	int parameterCount;
	int parameterSize;
	int localCount;
	int localSize;
	bool isExtern;
	bool isMethod;
	bool isGenerator;
	bool isVirtual;
	bool isBase;
	int virtualFunctionTableOffset;
	AccessModifierLevel AccessModifier;
};

// --------------------------------------------------------------------------------
//  Class allocation stuff.
// --------------------------------------------------------------------------------

// The header is the data stored on to of the object
// memory, it stores internally used members.
struct ClassObjectHeader
{
	// Keep this at the top - prevents padding and because
	// compiler assumes this is first.
	ClassDefinition* classDefinition; 

	// GC Code
	int* variableAddress;
	int classDefinitionIndex;
	int referenceCount;
	long long allocatedMemory;
	int alignmentPadding;
	int disposing;
};

// WARNING: Keep syncronized with the same value in Parser.h of compiler.
#define ClassHeaderSize 36//sizeof(ClassObjectHeader)

#define VFTABLE_TOSTRING_ENTRY	 2
#define VFTABLE_NEW_ENTRY		 1
#define VFTABLE_DELETE_ENTRY	 0

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------
extern std::vector<ClassDefinition*> g_classDefinitions;
extern MetaDataDefinition* g_globalDefinition;

extern std::list<ClassObjectHeader*> g_gcObjects;
extern std::list<ClassObjectHeader*> g_gcZeroRefObjects;
extern unsigned long long g_gcTotalAllocatedMemory;
extern unsigned long long g_gcAllocatedMemory;
extern unsigned long long g_gcFreedMemory;
extern unsigned long long g_gcZeroRefMemory;
extern unsigned long long g_gcZeroRefObjectCount;
extern bool g_gcPaused;

// Defines how much dead memory before we initiate a collection. (32kb)
#define GC_DEALLOCATE_DEAD_MEMORY_THRESHOLD (1024 * 32) // 1

// Defines how many dead objects before we initiate a collection. 
#define GC_DEALLOCATE_DEAD_OBJECT_THRESHOLD 300 // 1

// --------------------------------------------------------------------------------
//  Internal Functions.
// --------------------------------------------------------------------------------
MetaDataDefinition* GetMetaDataByName(const char* name, MetaDataDefinition* parent=NULL, int symbolType=META_TYPE_ANY);
void SetVariableValueByte(VariableDefinition* variable, ICR_BYTE value, ICR_INT thisPtr=NULL);
void SetVariableValueShort(VariableDefinition* variable, ICR_SHORT value, ICR_INT thisPtr=NULL);
void SetVariableValueInt(VariableDefinition* variable, ICR_INT value, ICR_INT thisPtr=NULL);
void SetVariableValueLong(VariableDefinition* variable, ICR_LONG value, ICR_INT thisPtr=NULL);
void SetVariableValueFloat(VariableDefinition* variable, ICR_FLOAT value, ICR_INT thisPtr=NULL);
void SetVariableValueDouble(VariableDefinition* variable, ICR_DOUBLE value, ICR_INT thisPtr=NULL);

ICR_BYTE GetVariableValueByte(VariableDefinition* variable, ICR_INT thisPtr=NULL);
ICR_SHORT GetVariableValueShort(VariableDefinition* variable, ICR_INT thisPtr=NULL);
ICR_INT GetVariableValueInt(VariableDefinition* variable, ICR_INT thisPtr=NULL);
ICR_LONG GetVariableValueLong(VariableDefinition* variable, ICR_INT thisPtr=NULL);
ICR_FLOAT GetVariableValueFloat(VariableDefinition* variable, ICR_INT thisPtr=NULL);
ICR_DOUBLE GetVariableValueDouble(VariableDefinition* variable, ICR_INT thisPtr=NULL);

// --------------------------------------------------------------------------------
//  External Assembly Functions
// --------------------------------------------------------------------------------
extern "C"
{
	ICR_INT icr__Class_ToString(ICR_INT ptrToObject);

	void icr__RegisterGlobal(MetaDataDefinition* ptrToGlobalDefinition);
	void icr__RegisterClass(ClassDefinition* ptrToClassDefinition);
	void icr__RegisterMetaData(MetaDataDefinition* ptrToClassDefinition);

	ICR_INT icr__AllocateObjectByClass(ClassDefinition* classType);
	ICR_INT icr__AllocateObject(ICR_INT classTableIndex);
	void icr__DeallocateObject(ICR_INT ptrToObject);
	    
	void icr__IncrementRefCount(ICR_INT ptrToObject);
	void icr__DecrementRefCount(ICR_INT ptrToObject);

	ICR_INT icr__ExplicitCast(ICR_INT destinationClassIndex, ICR_INT ptrToObject);

	void icr__BeginGC();
	void icr__FinishGC();
	void icr__ExecuteGC();
}


