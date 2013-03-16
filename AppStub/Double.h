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
	ICR_INT icr__Double_DoubleToInt(ICR_DOUBLE lvalue); 
	ICR_LONG icr__Double_DoubleToLong(ICR_DOUBLE lvalue); 
	StringObjectData* icr__Double_DoubleToString(ICR_DOUBLE lvalue); 
	ICR_FLOAT icr__Double_DoubleToFloat(ICR_DOUBLE lvalue); 
}