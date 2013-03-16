// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
//	This software is licensed under the terms provided in the LICENSE file 
//  contained within the root directory. If you did not get a LICENSE file with
//  this software please contact me at http://www.binaryphoenix.com
// --------------------------------------------------------------------------------
//  Icarus App Stub - This is the C app-stub that includes the standard runtime
//					  library that icarus operators off of. :3
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "AppStub.h"

#include "Debug.h"
#include "IO.h"

// Type-helper source code.
#include "Bool.h"
#include "Long.h"
#include "Int.h"
#include "Float.h"
#include "Class.h"
#include "String.h"

#include "Exception.h"

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------
AppType g_appType;
int g_stringClassTableIndex;

// --------------------------------------------------------------------------------
//  Console Application Start
// --------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	g_appType = APP_CONSOLE;
	
#ifdef DEBUG_OUTPUT
	printf("(AppStub: Entering _icr_main)\n");
#endif

	AllocExceptionStack();

	icr__BeginGC();
	_icr_main();
	icr__FinishGC();

	FreeExceptionStack();

#ifdef DEBUG_OUTPUT
	printf("(AppStub: Exited _icr_main)\n");
#endif

	return 0;
}

// --------------------------------------------------------------------------------
//  Windows Entry Point
// --------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	g_appType = APP_WINDOWS;
	
#ifdef DEBUG_OUTPUT
	printf("(AppStub: Entering _icr_main)\n");
#endif
	
	AllocExceptionStack();

	icr__BeginGC();
	_icr_main();
	icr__FinishGC();
	
	FreeExceptionStack();

#ifdef DEBUG_OUTPUT
	printf("(AppStub: Exited _icr_main)\n");
#endif

	return 0;
}

// --------------------------------------------------------------------------------
//  DLL Entry Point
// --------------------------------------------------------------------------------
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpReserved) 
{
	g_appType = APP_LIBRARY;
	
	AllocExceptionStack();
	icr__BeginGC();

    switch (fdwReason) 
    { 
        case DLL_PROCESS_ATTACH:
            break;

        case DLL_THREAD_ATTACH:
            break;

        case DLL_THREAD_DETACH:
            break;

        case DLL_PROCESS_DETACH:
            break;
    }
	
	icr__FinishGC();
	FreeExceptionStack();

    return true; 
}
