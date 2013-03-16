// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	Defines the different types of variables that can
//  be defined.
// --------------------------------------------------------------------------------
enum VariableType
{
	VT_GLOBAL = 0,
	VT_LOCAL,
	VT_CONST,
	VT_PARAMETER,
	VT_FIELD,
};

// --------------------------------------------------------------------------------
//	This class stores information on a variable symbol.
// --------------------------------------------------------------------------------
class VariableSymbol : public Symbol
{
//private:
public:
	DataType* DataType;
	VariableType VariableType;
	int StackOffset;
	bool UseParamStackOffset;
	int ParamStackOffset;
	int ClassOffset;
	bool IsConst;
	ClassSymbol* ParentClass;

	bool IsBase;

	AccessModifierLevel AccessModifier;

//public:
	VariableSymbol() { }
	VariableSymbol(std::string identifier, Symbol* scope, Token token)
	{
		StackOffset = 0;
		ParamStackOffset = 0;
		ClassOffset = 0;
		IsConst = false;
		AccessModifier = AM_PUBLIC;
		IsBase = false;
		ParentClass = NULL;
		Identifier = identifier;
		Scope = scope;
		DefinitionToken = token;
		UseParamStackOffset = false;
		if (scope != NULL)
			scope->Children.push_back(this);
	}

	void BuildClassHierarchy();

	SymbolType GetSymbolType() { return ST_VARIABLE; }

};