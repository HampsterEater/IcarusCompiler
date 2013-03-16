// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"

FunctionSymbol::FunctionSymbol(std::string identifier, Symbol* scope, Token token)
{
	IsBase = false;
	IsVirtual = false;
	IsExtern = false;
	IsMethod = false;
	IsGenerator = false;
	LocalCount = 0;
	LocalSize = 0;
	ParameterCount = 0;
	ParameterSize = 0;
	ReturnDataType = new VoidDataType();
	ReturnExists = false;
	IsVirtual = false;
	VirtualFunctionTableOffset = 0;
	IsBase = true;
	AccessModifier = AM_PUBLIC;

	DefinitionToken = token;
	Identifier = identifier;
	Scope = scope;
	if (scope != NULL)
		scope->Children.push_back(this);
}

void FunctionSymbol::BuildClassHierarchy()
{
	// Patch return-type if its an object.
	ClassDataType* classType = dynamic_cast<ClassDataType*>(ReturnDataType);
	if (classType != NULL && classType->Class == NULL)
		classType->Class = dynamic_cast<ClassSymbol*>(Scope->FindChild(classType->ClassName));
}
