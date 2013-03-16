// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

class FunctionSymbol;

// --------------------------------------------------------------------------------
//	This class stores information on a class symbol.
// --------------------------------------------------------------------------------
class ClassSymbol : public Symbol
{
//private:
public:
	int ClassDefinitionIndex;
	int MemoryDataSize;
	std::string ExtendsClassIdentifier;
	ClassSymbol* SuperClass;
	std::vector<ClassSymbol*> DerivedClasses;

	bool _hierarchyBuilt;
	
	int VirtualFunctionTableSize;

	FunctionSymbol* InternalConstructMethod;
	FunctionSymbol* InternalDisposeMethod;

//public:
	ClassSymbol() { }
	ClassSymbol(std::string identifier, Symbol* scope, Token token);
	
	void BuildClassHierarchy();
	void BuildVirtualFunctions();
	void BuildVariables();

	SymbolType GetSymbolType() { return ST_CLASS; }

};