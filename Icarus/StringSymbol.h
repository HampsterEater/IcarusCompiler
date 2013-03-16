// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	This class stores information on a variable symbol.
// --------------------------------------------------------------------------------
class StringSymbol : public Symbol
{
//private:
public:
	bool IsNullTerminated;

//public:
	StringSymbol() { IsNullTerminated = false; }
	StringSymbol(std::string identifier, Symbol* scope, Token token)
	{
		IsNullTerminated = false;
		Identifier = identifier;
		Scope = scope;
		DefinitionToken = token;
		if (scope != NULL)
			scope->Children.push_back(this);
	}

	void BuildClassHierarchy() { /* String symbols dosen't have to put up with this nonsense :3 */ }

	SymbolType GetSymbolType() { return ST_STRING; }

};