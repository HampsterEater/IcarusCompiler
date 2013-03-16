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
	ICR_LONG icr__LongDiv(ICR_LONG v2, ICR_LONG v1);
	ICR_LONG icr__LongMul(ICR_LONG v2, ICR_LONG v1);
	ICR_LONG icr__LongAdd(ICR_LONG v2, ICR_LONG v1);
	ICR_LONG icr__LongSub(ICR_LONG v2, ICR_LONG v1);

	ICR_INT icr__LongCNE(ICR_LONG v2, ICR_LONG v1);
	ICR_INT icr__LongCEQ(ICR_LONG v2, ICR_LONG v1);
	ICR_INT icr__LongCGE(ICR_LONG v2, ICR_LONG v1);
	ICR_INT icr__LongCLE(ICR_LONG v2, ICR_LONG v1);
	ICR_INT icr__LongCGT(ICR_LONG v2, ICR_LONG v1);
	ICR_INT icr__LongCLT(ICR_LONG v2, ICR_LONG v1);

	StringObjectData* icr__Long_LongToString(ICR_LONG lvalue);

	ICR_FLOAT icr__Long_LongToFloat(ICR_LONG lvalue);
	ICR_DOUBLE icr__Long_LongToDouble(ICR_LONG lvalue);
}


