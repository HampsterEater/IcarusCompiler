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
	StringObjectData* icr__Int_IntToString(ICR_INT lvalue);

	ICR_FLOAT icr__Int_IntToFloat(ICR_INT lvalue);
	ICR_DOUBLE icr__Int_IntToDouble(ICR_INT lvalue);
}