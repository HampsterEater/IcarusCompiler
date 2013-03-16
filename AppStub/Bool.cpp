// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "String.h"
#include "Bool.h"

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------


// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------

StringObjectData* icr__Bool_BoolToString(ICR_INT lvalue)
{
	//StringObjectData* ret = NULL;

	//printf("CONVERTING TO BOOL: %i\n", lvalue);
	if (lvalue == 0)
		return icr__String_CStringToString("False");
	else
		return icr__String_CStringToString("True");
	
	//char* a = icr__String_StringToCString(ret);
	//printf("GOT RESULT:%s\n", a);
	
	//return ret;
}