// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include <stdio.h>
#include <Windows.h>

// --------------------------------------------------------------------------------
//  Debug definitions.
// --------------------------------------------------------------------------------
//#define DEBUG_OUTPUT 1

// --------------------------------------------------------------------------------
//  Enumerations.
// --------------------------------------------------------------------------------
enum AppType // Defines what kind of application this is.
{
	APP_CONSOLE,
	APP_WINDOWS,
	APP_LIBRARY,
};

// --------------------------------------------------------------------------------
//  Global variables.
// --------------------------------------------------------------------------------
extern AppType g_appType;
extern int g_stringClassTableIndex;

// --------------------------------------------------------------------------------
//  Type declarations.
// --------------------------------------------------------------------------------
#define ICR_BYTE	unsigned char
#define ICR_BOOL	unsigned char
#define ICR_SHORT	unsigned short
#define ICR_INT		signed int
#define ICR_LONG	signed long long
#define ICR_FLOAT	float
#define ICR_DOUBLE	double
#define ICR_CLASS	int
#define ICR_STRING	int
#define ICR_STRING_PTR StringObjectData*

// --------------------------------------------------------------------------------
//  External Assembly Functions
// --------------------------------------------------------------------------------
extern "C" void _icr_main();

