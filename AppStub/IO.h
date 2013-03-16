// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <Windows.h>
#include <Winbase.h>

#include "AppStub.h"
#include "String.h"

// --------------------------------------------------------------------------------
//  External Assembly Functions
// --------------------------------------------------------------------------------
extern "C"
{
	void icr_Print(StringObjectData* val);
	void icr_PrintInt(int x);
	void icr_PrintNS(char* str);

	void icr_Sleep(ICR_INT n1);

	ICR_FLOAT icr_PerfTime();
}


