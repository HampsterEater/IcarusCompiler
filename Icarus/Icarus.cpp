// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
//	This software is licensed under the terms provided in the LICENSE file 
//  contained within the root directory. If you did not get a LICENSE file with
//  this software please contact me at http://www.binaryphoenix.com
// --------------------------------------------------------------------------------
//  Icarus Compiler - This console application is used to compile icarus (.ic)
//                    source code files into native executable files (.exe,.dll)
// --------------------------------------------------------------------------------

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
#include "Icarus.h"

// --------------------------------------------------------------------------------
//  Some random big ass todo list!
// --------------------------------------------------------------------------------
// TODO: Fix mem-leak with datatype.
// TODO: Array system?
// TODO: LongToString needs implementing look at print example
// TODO: Do strings based on length not on null termination!
// TODO: Remove string object and create script based one.
// TODO: Operating overloading, abstraction, virtual functions, etc.
// TODO: Convert sprintf's to sprintf_s's
// TODO: IntToFloat not working.
// TODO: Implement faster version of ref counting / gc collection
// TODO: Long always off by one
// TODO: Yielding functions and support for foreach
// TODO: Field pointers generated will be wrong as it now points directly to beginning.
// TODO: Optimize this pattern:
//			pop		eax
//			push	eax
// TODO: Add alias keyword.
// TODO: All the to-string methods should create one string object and sprintf directly into it. 
// TODO: Fix ++ and --
// TODO: Assigning to object variables (validation).
// Can't declare const's in fields or anything -> fix.
// TODO: A lot of casting operations (that use runtime functions) assume eax is free. May cause problems later!
// TODO: gc needs to decrement all object variable ref-counts when disposing
// TODO: Make switch parse statement not block.
// TODO: Remove vftable lookup for non-overriding methods.
// TODO: http://board.flatassembler.net/topic.php?t=677 (Icons and version info)
// TODO: Remove nulling code in constructor is variable has a default assignment.

// TODO: Turn semantics into errors not fatal errors!
// TODO: Iterators.
// TODO: Array/Dict/List
// TODO: Default parameters.
// TODO: Ternary Op
// TODO: Generator

// TODO: Make IntDataType/Long/etc have a flag "IsImmutable" if set then the value is a literal and the parser can work deconstruct it at runtime 4+3 in source is 7 in code.
// TODO: Pass class definition ptr rather than index when allocating.

// TODO: Check signed/unsigned arithemetic - specially with things like imul/idiv

// TODO: Reflection code (and implement ToString of class properly)

// TODO: Using

// Debug code!
// TODO: StackTrace API (register all functions/methods by address).

// TODO: DLL Load block.
//		 extern "kernel32.dll"
//		 {
//			 void OpenGame(int x, int y);
//		 }

// --------------------------------------------------------------------------------
//  Application Entry Point
// --------------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	g_app = new IcarusCompilerApp();

	if (g_app->ParseCommandLines(argc, argv) == true)
	{
		if (g_app->Compile() == true)
		{
			delete g_app;
			return 0;
		}
	}

	delete g_app;
	return 1;
}

