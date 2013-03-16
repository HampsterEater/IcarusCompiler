// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "String.h"
#include "Float.h"

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------
StringObjectData* icr__Float_FloatToString(ICR_FLOAT lvalue)
{
	char* result = new char[256];
	sprintf(result, "%f", lvalue);

	StringObjectData* realResult = icr__String_CStringToString(result);

	delete[] result;
	return realResult;
}

ICR_INT icr__Float_FloatToInt(ICR_FLOAT lvalue)
{
	return (ICR_INT)lvalue;
}

ICR_LONG icr__Float_FloatToLong(ICR_FLOAT lvalue)
{
	return (ICR_LONG)lvalue;
}

ICR_DOUBLE icr__Float_FloatToDouble(ICR_FLOAT lvalue)
{
	return (ICR_DOUBLE)lvalue;
}

