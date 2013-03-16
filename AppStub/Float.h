// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <Windows.h>

#include "AppStub.h"

#include "String.h"

// --------------------------------------------------------------------------------
//  External Assembly Functions
// --------------------------------------------------------------------------------
extern "C"
{	
	StringObjectData* icr__Float_FloatToString(ICR_FLOAT lvalue);
	
	ICR_INT icr__Float_FloatToInt(ICR_FLOAT lvalue);
	ICR_LONG icr__Float_FloatToLong(ICR_FLOAT lvalue);
	ICR_DOUBLE icr__Float_FloatToDouble(ICR_FLOAT lvalue);
}