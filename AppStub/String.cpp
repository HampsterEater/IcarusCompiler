// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "String.h"
#include "Class.h"

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------

StringObjectData* icr__String_CreateFromLiteral(int* ptrToMemory)
{
	int strLength = *((int*)ptrToMemory);
	char* stringRawData = (char*)(ptrToMemory + 1);

	ICR_INT allocatedObject = icr__AllocateObject(g_stringClassTableIndex);

	StringObjectData* stringData = (StringObjectData*)((int*)allocatedObject);
	stringData->dataLength = strLength;
	stringData->ptrToData = new char[strLength];
	
	memcpy(stringData->ptrToData, stringRawData, strLength);
	
	return (StringObjectData*)((int*)allocatedObject);
}

StringObjectData* icr__String_Concat(StringObjectData* ptrToStringTwo, StringObjectData* ptrToStringOne)
{
	int totalLength = ptrToStringOne->dataLength + ptrToStringTwo->dataLength;

	ICR_INT allocatedObject = icr__AllocateObject(g_stringClassTableIndex);

	StringObjectData* stringData = (StringObjectData*)((int*)allocatedObject);
	stringData->dataLength = totalLength;
	stringData->ptrToData = new char[totalLength];
	
	memcpy(stringData->ptrToData, ptrToStringOne->ptrToData, ptrToStringOne->dataLength);
	memcpy((stringData->ptrToData + ptrToStringOne->dataLength), ptrToStringTwo->ptrToData, ptrToStringTwo->dataLength);

	//char* a = icr__String_StringToCString(stringData);
	//char* b = icr__String_StringToCString(stringData);
	//char* t = icr__String_StringToCString(stringData);
	//printf("A: %s\n", a);
	//printf("B: %s\n", b);
	//printf("Concated To: %s\n", t);

	return (StringObjectData*)((int*)allocatedObject);
}

char* icr__String_StringToCString(StringObjectData* ptrToStringOne)
{
	int lengthToNull = 0;

	// Count the length of data to the null.
	for (int i = 0; i < ptrToStringOne->dataLength; i++)
	{
		if (ptrToStringOne->ptrToData[i] == '\0')
			break;
		else
			lengthToNull++;
	}

	char* stringData = new char[lengthToNull + 1];
	memcpy(stringData, ptrToStringOne->ptrToData, lengthToNull);

	stringData[lengthToNull] = '\0';

	return stringData;
}

StringObjectData* icr__String_CStringToString(char* ptrToStringOne)
{
	int strLength = strlen(ptrToStringOne);
	char* stringRawData = ptrToStringOne;

	ICR_INT allocatedObject = icr__AllocateObject(g_stringClassTableIndex);

	StringObjectData* stringData = (StringObjectData*)((int*)allocatedObject);
	stringData->dataLength = strLength;
	stringData->ptrToData = new char[strLength];
	
	memcpy(stringData->ptrToData, stringRawData, strLength);
	
	return (StringObjectData*)((int*)allocatedObject);
}

void icr__String_FreeCString(char* ptrToCString)
{
	delete[] ptrToCString;
}

ICR_INT icr__String_CNE(StringObjectData* rvalue, StringObjectData* lvalue)
{
	bool isEqual = true;

	if (rvalue->dataLength != lvalue->dataLength)
		isEqual = false;
	else
	{
		for (int i = 0; i < lvalue->dataLength; i++)
		{
			if (lvalue->ptrToData[i] != rvalue->ptrToData[i])
			{
				isEqual = false;
				break;
			}
		}
	}

	return !isEqual;
}

ICR_INT icr__String_CEQ(StringObjectData* rvalue, StringObjectData* lvalue)
{
	bool isEqual = true;

	if (rvalue->dataLength != lvalue->dataLength)
		isEqual = false;
	else
	{
		for (int i = 0; i < lvalue->dataLength; i++)
		{
			if (lvalue->ptrToData[i] != rvalue->ptrToData[i])
			{
				isEqual = false;
				break;
			}
		}
	}

	return isEqual;
}

ICR_INT icr__String_StringToBool(StringObjectData* lvalue)
{
	bool result = true;
	char* val = icr__String_StringToCString(lvalue);

	if (stricmp(val, "false") == 0 || stricmp(val, "0") == 0)
		result = false;
	 
	icr__String_FreeCString(val);

	return result == true ? 1 : 0;
}

ICR_INT icr__String_StringToInt(StringObjectData* lvalue)
{
	int result = 0;
	char* val = icr__String_StringToCString(lvalue);

	result = atoi(val); 

	icr__String_FreeCString(val);

	return result;
}

ICR_LONG icr__String_StringToLong(StringObjectData* lvalue)
{
	long long result = 0;
	char* val = icr__String_StringToCString(lvalue);

	result = _atoi64(val);

	icr__String_FreeCString(val);

	return result;
}

ICR_FLOAT icr__String_StringToFloat(StringObjectData* lvalue)
{
	float result = 0;
	char* val = icr__String_StringToCString(lvalue);

	result = strtod(val, NULL); 

	icr__String_FreeCString(val);

	return result;
}

ICR_DOUBLE _String_StringToDouble(StringObjectData* lvalue)
{
	double result = 0;
	char* val = icr__String_StringToCString(lvalue);

	result = strtod(val, NULL); 

	icr__String_FreeCString(val);

	return result;
}