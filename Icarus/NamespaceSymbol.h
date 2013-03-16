// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	This class stores information on a variable symbol.
// --------------------------------------------------------------------------------
class NamespaceSymbol : public Symbol
{
//private:
public:
	
//public:
	NamespaceSymbol() { }
	NamespaceSymbol(std::string identifier, Symbol* scope, Token token)
	{
		Identifier = identifier;
		Scope = scope;
		DefinitionToken = token;
		if (scope != NULL)
			scope->Children.push_back(this);
	}

	void BuildClassHierarchy() { /* Namespace symbols dosen't have to put up with this nonsense :3 */ }

	SymbolType GetSymbolType() { return ST_NAMESPACE; }

};