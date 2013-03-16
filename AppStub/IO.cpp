// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "IO.h"

// --------------------------------------------------------------------------------
//  Exported functions.
// --------------------------------------------------------------------------------

void icr_Print(StringObjectData* val)
{
	char* cStringPtr = icr__String_StringToCString(val);	
	printf(cStringPtr);
	icr__String_FreeCString(cStringPtr);
}

void icr_PrintInt(int x)
{
	printf("VAL:%i\n", x);
}

void icr_PrintNS(char* str)
{
	printf("VAL:%s\n", str);
}

void icr_Sleep(ICR_INT n1)
{
	//printf("Sleeping: %i\n", n1);
	Sleep(n1);
}


ICR_FLOAT icr_PerfTime()
{
	static LARGE_INTEGER frequency;
	static LARGE_INTEGER start;
	static int initialized = 0;

	if (initialized == 0)
	{
		QueryPerformanceFrequency(&frequency);
		QueryPerformanceCounter(&start);
		initialized = 1;
	}

	LARGE_INTEGER time;
	QueryPerformanceCounter(&time);

	return (ICR_FLOAT(time.QuadPart - start.QuadPart) / ICR_FLOAT(frequency.QuadPart)) * 1000.0;
}