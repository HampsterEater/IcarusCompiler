// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#include "Exception.h"
#include <string>

#include "String.h"

#include "Class.h"

// --------------------------------------------------------------------------------
//  Internal variables.
// --------------------------------------------------------------------------------
std::vector<ExceptionStackEntry*> g_exceptionStack;

// Called when the code enters a "Try" block.
//
// Returns: Exceptin block to be filled with the CPU state.
void* icr__ExceptionStackEnter()
{	
	ExceptionStackEntry* block = new ExceptionStackEntry();
	g_exceptionStack.push_back(block);

	return block->_cpu_state;
}

// Called when the code leaves a "Try" block without an exception.
//
// Returns nothing!
void icr__ExceptionStackLeave()
{
	ExceptionStackEntry* block = g_exceptionStack.back();
	g_exceptionStack.pop_back();

	delete block;
}

// Throws the given icarus object as an exception!
void icr__ExceptionThrow(ICR_INT ptrToObject)
{
	// If exception stack is empty, we are unhandled!
	if (g_exceptionStack.size() <= 0)
		UnhandledException(ptrToObject);

	// Get the state of the CPU from last exception handler.
	ExceptionStackEntry* block = g_exceptionStack.back();
	g_exceptionStack.pop_back();

	// Copy block buffer into cpu memory, as we're about to delete it.
	static char buffer[CPU_STATE_SIZE];
	memcpy(buffer, block->_cpu_state, CPU_STATE_SIZE);

	// Dispose of block.
	delete block;

	// Return the CPU to the last exception state!
	icr__ExceptionSetCPUState(buffer, ptrToObject);

	// Kind of redundent, we won't be getting here :3.
	return;
}

// This is used by the windows unhandled exception filter. It
// sets the state values in the context for restoring the state, but 
// dosen't jump back to the old execution position.
void icr__ExceptionThrowUsingContext(ICR_INT ptrToObject, PCONTEXT context)
{
	// If exception stack is empty, we are unhandled!
	if (g_exceptionStack.size() <= 0)
		UnhandledException(ptrToObject);

	// Get the state of the CPU from last exception handler.
	ExceptionStackEntry* block = g_exceptionStack.back();
	g_exceptionStack.pop_back();

	// Copy block buffer into cpu memory, as we're about to delete it.
	char* buffer = (char*)block->_cpu_state;

	// Return the CPU to the last exception state!
	context->Edx = (int)block;
	context->Eax = (int)ptrToObject;

	context->Ebp = *((int*)(buffer + 16));
	context->Edi = *((int*)(buffer + 12));
	context->Esi = *((int*)(buffer + 8));
	context->Ebx = *((int*)(buffer + 4));
	context->Esp = *((int*)(buffer + 20)) + 4;
	context->Eip = *((int*)(buffer + 24)); // Ret

	return;
}

// Allocates the exception stack, called on startup.
void AllocExceptionStack()
{
	SetUnhandledExceptionFilter(BaseUnhandledExceptionFilter);
}

// Frees the exception stack, called on shutdown.
void FreeExceptionStack()
{
	g_exceptionStack.clear();
}

// Invoked when an unhandled exception is called.
void UnhandledException(ICR_INT ptrToObject)
{
	char* exceptionName = "Unknown or Null";
	int address = 123512;

	if (ptrToObject != NULL)
	{
		ClassObjectHeader* objPtr = (ClassObjectHeader*)(ptrToObject - ClassHeaderSize);
		exceptionName = objPtr->classDefinition->ptrToName;
		
		VariableDefinition* field = (VariableDefinition*)GetMetaDataByName("Address",  (MetaDataDefinition*)objPtr->classDefinition, META_TYPE_VARIABLE);
		address = GetVariableValueInt(field, ptrToObject);
	}

	char msg[512];
	sprintf(msg, "An unhandled exception occured during execution.\n" \
				 "This program is unable to continue, please contact the developers.\n" \
				 "\n" \
				 "Type:\t%s\n" \
				 "Address:\t0x%p\n",
				 exceptionName, address);

	MessageBoxA(GetActiveWindow(), msg, "Unhandled Exception", MB_OK);
	exit(0);
}

// Invoked by the windows API when an unhandled exception occurs.
LONG WINAPI BaseUnhandledExceptionFilter(EXCEPTION_POINTERS *xinfo)
{
	const char* exceptionName = "UnknownSystemException";
	switch (xinfo->ExceptionRecord->ExceptionCode)
	{
		case EXCEPTION_ACCESS_VIOLATION:			exceptionName = "AccessViolationException";					break;
		case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:		exceptionName = "ArrayBoundsExceededException";				break;
		case EXCEPTION_BREAKPOINT:					exceptionName = "BreakpointException";						break;
		case EXCEPTION_DATATYPE_MISALIGNMENT:		exceptionName = "DataTypeMisalignmentException";			break;
		case EXCEPTION_FLT_DENORMAL_OPERAND:		exceptionName = "FloatDenormalOperandException";			break;
		case EXCEPTION_FLT_DIVIDE_BY_ZERO:			exceptionName = "FloatDivideByZeroException";				break;
		case EXCEPTION_FLT_INEXACT_RESULT:			exceptionName = "FloatInexactResultException";				break;
		case EXCEPTION_FLT_INVALID_OPERATION:		exceptionName = "FloatInvalidOperationException";			break;
		case EXCEPTION_FLT_OVERFLOW:				exceptionName = "FloatOverflowException";					break;
		case EXCEPTION_FLT_STACK_CHECK:				exceptionName = "FloatStackCheckException";					break;
		case EXCEPTION_FLT_UNDERFLOW:				exceptionName = "FloatUnderflowException";					break;
		case EXCEPTION_ILLEGAL_INSTRUCTION:			exceptionName = "IllegalInstructionException";				break;
		case EXCEPTION_IN_PAGE_ERROR:				exceptionName = "PageErrorException";						break;
		case EXCEPTION_INT_DIVIDE_BY_ZERO:			exceptionName = "DivideByZeroException";					break;
		case EXCEPTION_INT_OVERFLOW:				exceptionName = "OverflowException";						break;
		case EXCEPTION_INVALID_DISPOSITION:			exceptionName = "InvalidDispositionException";				break;
		case EXCEPTION_NONCONTINUABLE_EXCEPTION:	exceptionName = "NonContinuableException";					break;
		case EXCEPTION_PRIV_INSTRUCTION:			exceptionName = "PrivInstructionException";					break;
		case EXCEPTION_SINGLE_STEP:					exceptionName = "SingleStepException";						break;
		case EXCEPTION_STACK_OVERFLOW:				exceptionName = "StackOverflowException";					break;
	}

	// Allocate and throw the exception class.
	ClassDefinition* exceptionClass = (ClassDefinition*)GetMetaDataByName(exceptionName, NULL, META_TYPE_CLASS);
	if (exceptionClass != NULL)
	{		
		ICR_INT exceptionObjPtr = icr__AllocateObjectByClass(exceptionClass);
		if (exceptionObjPtr != NULL)
		{
			VariableDefinition* field = (VariableDefinition*)GetMetaDataByName("Address",  (MetaDataDefinition*)exceptionClass, META_TYPE_VARIABLE);
			SetVariableValueInt(field, (int)xinfo->ExceptionRecord->ExceptionAddress, exceptionObjPtr);

			// Set fields of the exception.
			switch (xinfo->ExceptionRecord->ExceptionCode)
			{
				case EXCEPTION_ACCESS_VIOLATION:	
					field = (VariableDefinition*)GetMetaDataByName("ReadWriteFlag", (MetaDataDefinition*)exceptionClass, META_TYPE_VARIABLE);
					SetVariableValueInt(field, (int)xinfo->ExceptionRecord->ExceptionInformation[0], exceptionObjPtr);

					field = (VariableDefinition*)GetMetaDataByName("AccessAddress", (MetaDataDefinition*)exceptionClass, META_TYPE_VARIABLE);
					SetVariableValueInt(field, (int)xinfo->ExceptionRecord->ExceptionInformation[1], exceptionObjPtr);
					break;	

				case EXCEPTION_IN_PAGE_ERROR:	
					field = (VariableDefinition*)GetMetaDataByName("ReadWriteFlag", (MetaDataDefinition*)exceptionClass, META_TYPE_VARIABLE);
					SetVariableValueInt(field, xinfo->ExceptionRecord->ExceptionInformation[0], exceptionObjPtr);

					field = (VariableDefinition*)GetMetaDataByName("AccessAddress", (MetaDataDefinition*)exceptionClass, META_TYPE_VARIABLE);
					SetVariableValueInt(field, xinfo->ExceptionRecord->ExceptionInformation[1], exceptionObjPtr);	

					field = (VariableDefinition*)GetMetaDataByName("NTSTATUSCode", (MetaDataDefinition*)exceptionClass, META_TYPE_VARIABLE);
					SetVariableValueInt(field, xinfo->ExceptionRecord->ExceptionInformation[2], exceptionObjPtr);			
					break;	
			}

			// Throw our new exception object!
			icr__ExceptionThrowUsingContext(exceptionObjPtr, xinfo->ContextRecord);
			return EXCEPTION_CONTINUE_EXECUTION;
		}
	}

	// Failed? Throw an error :(.
	UnhandledException(0);
}
