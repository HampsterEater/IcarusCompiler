// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "String.h"
#include "Int.h"

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------

StringObjectData* icr__Int_IntToString(ICR_INT lvalue)
{
	char* result = new char[256];
	itoa(lvalue, result, 10);
	
	StringObjectData* realResult = icr__String_CStringToString(result);

	delete[] result;
	return realResult;
}

ICR_FLOAT icr__Int_IntToFloat(ICR_INT lvalue)
{
	return (ICR_FLOAT)lvalue;
}

ICR_DOUBLE icr__Int_IntToDouble(ICR_INT lvalue)
{
	return (ICR_DOUBLE)lvalue;
}
