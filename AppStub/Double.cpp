// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "String.h"
#include "Double.h"

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------

ICR_INT icr__Double_DoubleToInt(ICR_DOUBLE lvalue)
{
	return (ICR_INT)lvalue;
}

ICR_LONG icr__Double_DoubleToLong(ICR_DOUBLE lvalue)
{
	return (ICR_LONG)lvalue;
}

StringObjectData* icr__Double_DoubleToString(ICR_DOUBLE lvalue)
{	
	char* result = new char[256];
	sprintf(result, "%f", lvalue);
	
	StringObjectData* realResult = icr__String_CStringToString(result);

	delete[] result;
	return realResult;
}

ICR_FLOAT icr__Double_DoubleToFloat(ICR_DOUBLE lvalue)
{
	return (ICR_FLOAT)lvalue;
}