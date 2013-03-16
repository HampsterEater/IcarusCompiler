// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

class FunctionSymbol;

// --------------------------------------------------------------------------------
//  Stores an access modifier.
// --------------------------------------------------------------------------------
enum AccessModifierLevel
{
	AM_PUBLIC = 0,
	AM_PRIVATE,
	AM_PROTECTED
};

// --------------------------------------------------------------------------------
//	This enum contains unique identifiers for each type of symbol.
// --------------------------------------------------------------------------------
enum SymbolType
{
	ST_FUNCTION = 0,
	ST_VARIABLE,
	ST_CLASS,
	ST_STRING,
	ST_NAMESPACE
};

// --------------------------------------------------------------------------------
//	This is the base code class for a code symbol, which describes
//  a logical element within source code (eg, a function, a class, etc).
// --------------------------------------------------------------------------------
class Symbol
{
public:
//private:
	//SymbolType Type;
	std::string Identifier;
	std::string UniqueASMName;
	Symbol* Scope;
	Token DefinitionToken;

	std::vector<Symbol*> Children;
	std::vector<Instruction*> Instructions;

	bool GeneratedASMName;
	bool GeneratedSymbolData;
	bool GeneratedSymbolCode;
	bool GeneratedSymbolHeaderExtern;
	bool GeneratedSymbolHeader;

//public:
	Symbol()
	{
		GeneratedASMName = false;		
		GeneratedSymbolData = false;
		GeneratedSymbolCode = false;
		GeneratedSymbolHeader = false;
		GeneratedSymbolHeaderExtern = false;
	}
	Symbol(std::string identifier, Symbol* scope, Token token)
	{
		GeneratedASMName = false;		
		GeneratedSymbolData = false;
		GeneratedSymbolCode = false;
		GeneratedSymbolHeader = false;
		GeneratedSymbolHeaderExtern = false;

		Identifier = identifier;
		Scope = scope;
		DefinitionToken = token;
		if (scope != NULL)
			scope->Children.push_back(this);
	}
	virtual SymbolType GetSymbolType() = 0;
	virtual void BuildClassHierarchy() = 0;

	Symbol* FindChild(std::string name, bool caseSensitive=false, bool recurse=true);
	Symbol* FindString(std::string name, bool isNullTerminated=false);

	FunctionSymbol* FindChildFunction(std::string name, int paramCount, bool caseSensitive=false, bool recurse=true);

	int GetInstructionIndex(Instruction* instruction);
};