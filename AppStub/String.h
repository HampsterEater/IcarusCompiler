// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <Windows.h>

#include "AppStub.h"

#define STRING_CLASS_MEMORY_SIZE 8

// --------------------------------------------------------------------------------
//  String definition.
// --------------------------------------------------------------------------------
struct StringObjectData
{
	int dataLength;
	char* ptrToData;
};

// --------------------------------------------------------------------------------
//  External Assembly Functions
// --------------------------------------------------------------------------------
extern "C"
{	
	StringObjectData* icr__String_CreateFromLiteral(int* ptrToMemory);
	StringObjectData* icr__String_Concat(StringObjectData* ptrToStringOne, StringObjectData* ptrToStringTwo);
	
	char* icr__String_StringToCString(StringObjectData* ptrToStringOne);
	StringObjectData* icr__String_CStringToString(char* ptrToStringOne);
	void icr__String_FreeCString(char* ptrToCString);
	
	ICR_INT icr__String_CNE(StringObjectData* rvalue, StringObjectData* lvalue);
	ICR_INT icr__String_CEQ(StringObjectData* rvalue, StringObjectData* lvalue);

	ICR_INT icr__String_StringToBool(StringObjectData* lvalue);
	ICR_INT icr__String_StringToInt(StringObjectData* lvalue);
	ICR_LONG icr__String_StringToLong(StringObjectData* lvalue);
	ICR_FLOAT icr__String_StringToFloat(StringObjectData* lvalue);

	ICR_DOUBLE _String_StringToDouble(StringObjectData* lvalue);  

	//void icr__String_ClassToString(int lvalue);
}


