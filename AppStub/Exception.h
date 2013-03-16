// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <Windows.h>
#include <vector>
#include <list>

#include "AppStub.h"

#define CPU_STATE_SIZE 256

// Stores an entry in the exception stack.
struct ExceptionStackEntry
{
	// Stores the CPU state.
	char _cpu_state[CPU_STATE_SIZE];
};

// --------------------------------------------------------------------------------
//  Internal variables.
// --------------------------------------------------------------------------------
extern std::vector<ExceptionStackEntry*> g_exceptionStack;

// --------------------------------------------------------------------------------
//  Internal functions.
// --------------------------------------------------------------------------------
LONG WINAPI BaseUnhandledExceptionFilter(EXCEPTION_POINTERS *xinfo);
void AllocExceptionStack();
void FreeExceptionStack();
void UnhandledException(ICR_INT ptrToObject);

// --------------------------------------------------------------------------------
//  External Assembly Functions
// --------------------------------------------------------------------------------
extern "C"
{
	// C Functions
	void* icr__ExceptionStackEnter();
	void icr__ExceptionStackLeave();
	void icr__ExceptionThrow(ICR_INT ptrToObject);
	void icr__ExceptionThrowUsingContext(ICR_INT ptrToContext, PCONTEXT context);

	// ASM functions
	ICR_INT icr__ExceptionSetCPUState(void* cpuState, ICR_INT ptrToObject);
	void icr__ExceptionGetCPUState(void* cpuState);
}



