// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"

// --------------------------------------------------------------------------------
//  Finds a child symbol held by the current symbol. This
//  recursively checks further up the scope tree until we get to the global scope.
// --------------------------------------------------------------------------------
Symbol* Symbol::FindChild(std::string name, bool caseSensitive, bool recurse)
{
	// Check if any of our symbols are what we are after.
	for (int i = 0; i < (int)Children.size(); i++)
	{
		Symbol* sym = Children.at(i);
		if (sym->GetSymbolType() != ST_STRING && 
			((caseSensitive == true && sym->Identifier == name) || 
			(caseSensitive == false && StringHelper::ToLower(sym->Identifier) == StringHelper::ToLower(name))))
		{
			return sym;
		}
	}

	// Check the parent scope.
	if (Scope != NULL && recurse == true)
		return Scope->FindChild(name, caseSensitive, recurse);
	
	// Nothing :(
	return NULL;
}

// --------------------------------------------------------------------------------
//  Works the same as FindChild except only returns functions and only ones with a 
//  certain parameter count.
// --------------------------------------------------------------------------------
FunctionSymbol* Symbol::FindChildFunction(std::string name, int paramCount, bool caseSensitive, bool recurse)
{
	// Check if any of our symbols are what we are after.
	for (int i = 0; i < (int)Children.size(); i++)
	{
		Symbol* sym = Children.at(i);
		if (sym->GetSymbolType() == ST_FUNCTION && 
			((FunctionSymbol*)sym)->ParameterCount == paramCount &&
			((caseSensitive == true && sym->Identifier == name) || 
			(caseSensitive == false && StringHelper::ToLower(sym->Identifier) == StringHelper::ToLower(name))))
		{
			return (FunctionSymbol*)sym;
		}
	}

	// Check the parent scope.
	if (Scope != NULL && recurse == true)
		return Scope->FindChildFunction(name, paramCount, caseSensitive, recurse);
	
	// Nothing :(
	return NULL;
}

// --------------------------------------------------------------------------------
//  Same as find child basically, but only deals with string symbols.
// --------------------------------------------------------------------------------
Symbol* Symbol::FindString(std::string name, bool isNullTerminated)
{
	// Check if any of our symbols are what we are after.
	for (int i = 0; i < (int)Children.size(); i++)
	{
		Symbol* sym = Children.at(i);
		if (sym->GetSymbolType() == ST_STRING && 
			((StringSymbol*)sym)->IsNullTerminated == isNullTerminated && 
			 StringHelper::ToLower(sym->Identifier) == StringHelper::ToLower(name))
		{
			return sym;
		}
	}

	// Check the parent scope.
	if (Scope != NULL)
		return Scope->FindString(name, isNullTerminated);
	
	// Nothing :(
	return NULL;
}

// --------------------------------------------------------------------------------
//  Gets the index of an instruction.
// --------------------------------------------------------------------------------
int Symbol::GetInstructionIndex(Instruction* instruction)
{
	for (int i = 0; i < Instructions.size(); i++)
		if (Instructions.at(i) == instruction)
			return i;
	return -1;
}