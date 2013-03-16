// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "Long.h"

// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------

ICR_LONG icr__LongDiv(ICR_LONG v2, ICR_LONG v1)
{
	return v1 / v2;
}
ICR_LONG icr__LongMul(ICR_LONG v2, ICR_LONG v1)
{
	return v1 * v2;
}
ICR_LONG icr__LongAdd(ICR_LONG v2, ICR_LONG v1)
{
	return v1 + v2;
}
ICR_LONG icr__LongSub(ICR_LONG v2, ICR_LONG v1)
{
	return v1 - v2;
}

ICR_INT icr__LongCNE(ICR_LONG v2, ICR_LONG v1)
{
	return v1 != v2 ? 1 : 0;
}
ICR_INT icr__LongCEQ(ICR_LONG v2, ICR_LONG v1)
{
	return v1 == v2 ? 1 : 0;
}
ICR_INT icr__LongCGE(ICR_LONG v2, ICR_LONG v1)
{
	return v1 >= v2 ? 1 : 0;
}
ICR_INT icr__LongCLE(ICR_LONG v2, ICR_LONG v1)
{
	return v1 <= v2 ? 1 : 0;
}
ICR_INT icr__LongCGT(ICR_LONG v2, ICR_LONG v1)
{
	return v1 > v2 ? 1 : 0;
}
ICR_INT icr__LongCLT(ICR_LONG v2, ICR_LONG v1)
{
	return v1 < v2 ? 1 : 0;
}

StringObjectData* icr__Long_LongToString(ICR_LONG lvalue)
{
	char* result = new char[256];
	sprintf(result, "%lld", lvalue);

	StringObjectData* realResult = icr__String_CStringToString(result);

	delete[] result;
	return realResult;
}

ICR_FLOAT icr__Long_LongToFloat(ICR_LONG lvalue)
{	
	return (ICR_FLOAT)lvalue;
}

ICR_DOUBLE icr__Long_LongToDouble(ICR_LONG lvalue)
{	
	return (ICR_DOUBLE)lvalue;
}

