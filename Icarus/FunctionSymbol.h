// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	This class stores information on a function symbol.
// --------------------------------------------------------------------------------
class FunctionSymbol : public Symbol
{
//private:
public:
	DataType* ReturnDataType;
	int ParameterCount;
	int ParameterSize;

	int LocalCount;
	int LocalSize;
	
	bool IsExtern;
	bool IsMethod;
	bool IsGenerator;

	bool ReturnExists;

	bool IsVirtual;
	bool IsBase;

	bool CodeGenerated;

	int VirtualFunctionTableOffset;

	AccessModifierLevel AccessModifier;

//public:
	FunctionSymbol() { AccessModifier = AM_PUBLIC; }
	FunctionSymbol(std::string identifier, Symbol* scope, Token token);

	void BuildClassHierarchy();

	SymbolType GetSymbolType() { return ST_FUNCTION; }

};