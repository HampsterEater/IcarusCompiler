// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"

ClassSymbol::ClassSymbol(std::string identifier, Symbol* scope, Token token)
{
	Identifier = identifier;
	Scope = scope;
	DefinitionToken = token;
	MemoryDataSize = 0;
	ExtendsClassIdentifier = "";
	SuperClass = NULL;
	_hierarchyBuilt = false;
	VirtualFunctionTableSize = 0;
	InternalConstructMethod = NULL;
	InternalDisposeMethod = NULL;

	if (scope != NULL)
		scope->Children.push_back(this);
}

// Build the class heirarchy.
void ClassSymbol::BuildClassHierarchy()
{
	if (_hierarchyBuilt == true)
		return;

	_hierarchyBuilt = true;

	ClassSymbol* extensionClass = dynamic_cast<ClassSymbol*>(Scope->FindChild(ExtendsClassIdentifier));
	if (extensionClass == NULL)
	{
		if (ExtendsClassIdentifier != "")
			g_app->EmitError("Undefined super class \"" + ExtendsClassIdentifier + "\".", ES_PARSER, EL_FATAL, DefinitionToken.Line, DefinitionToken.Column, DefinitionToken.Path);
	}
	else
	{
		SuperClass = extensionClass;
		SuperClass->DerivedClasses.push_back(this);

		// Right, build the inheretance tree.
		std::vector<ClassSymbol*> tree;
		ClassSymbol* treeParent = this;
		while (treeParent != NULL)
		{
			tree.push_back(treeParent);

			if (treeParent->_hierarchyBuilt == false)
				treeParent->BuildClassHierarchy();

			treeParent = treeParent->SuperClass;
		}
	}
}

// Builds the variables table.
void ClassSymbol::BuildVariables()
{
	// Set out virtual table size to start with.
	MemoryDataSize = SuperClass == NULL ? 0 : SuperClass->MemoryDataSize;

	//printf("-- Class:%s --\n", this->Identifier.c_str());

	// Check for any base variables in ourselves and if so increment the data suze and
	// assign allocations.
	for (int i = 0 ; i < Children.size(); i++)
	{
		VariableSymbol* var = dynamic_cast<VariableSymbol*>(Children.at(i));
		if (var == NULL) continue;
		if (var->VariableType != VT_FIELD) continue;

		// Work out if its a base by checking if parent has a copy of it (as it copies its parents ones it should be recursive).
		var->IsBase = false;
		if (SuperClass == NULL || SuperClass->FindChild(var->Identifier, false, false) == NULL)
			var->IsBase = true;

		if (var != NULL && var->IsBase == true && var->Scope == this)
		{
			var->ClassOffset = MemoryDataSize;
			var->ParentClass = this;
			MemoryDataSize += var->DataType->GetStoredSize();
			//printf("     Var:%s - %i --\n", this->Identifier.c_str(), var->ClassOffset);
		}
	}

	// Copy base fields from all parent classes.
	ClassSymbol* parent = SuperClass;
	if (parent != NULL)
	{
		for (int i = 0 ; i < parent->Children.size(); i++)
		{
			VariableSymbol* var = dynamic_cast<VariableSymbol*>(parent->Children.at(i));
			if (var == NULL) continue;
			if (var->VariableType != VT_FIELD) continue;

			// Check function dosen't already exist, if it does then assign this func
			// the same virtual function table index.
			VariableSymbol* ourVar = dynamic_cast<VariableSymbol*>(FindChild(var->Identifier, false, false));
			if (ourVar != NULL)
			{
				// Variable already defined.
				g_app->EmitError("Member variable \"" + var->Identifier + "\" already defined in super-class \"" + parent->Identifier + "\".", ES_PARSER, EL_FATAL, var->DefinitionToken.Line, var->DefinitionToken.Column, var->DefinitionToken.Path);
			}
			else if (var != NULL && var->IsBase == true)
			{
				Children.push_back(var);
			}
		}
	}
	//printf("## SIZE %i ## \n\n", this->MemoryDataSize);

	// Go through derived classes and build them too.
	for (int i = 0; i < this->DerivedClasses.size(); i++)
	{
		ClassSymbol* child = this->DerivedClasses.at(i);
	
		// Build the virtual childs functions using us as a base.
		child->BuildVariables();
	}
}

// Builds the virtual function table of awesome :3.
void ClassSymbol::BuildVirtualFunctions()
{
	// Set out virtual table size to start with.
	VirtualFunctionTableSize = SuperClass == NULL ? 0 : SuperClass->VirtualFunctionTableSize;

	// Check for any base methods in ourselves and if so increment the virtual table size and
	// assign allocations.
	//printf("\nCalculating %s...\n", Identifier.c_str());
	for (int i = 0 ; i < Children.size(); i++)
	{
		FunctionSymbol* func = dynamic_cast<FunctionSymbol*>(Children.at(i));
		if (func == NULL) continue;

		// Work out if its a base by checking if parent has a copy of it (as it copies its parents ones it should be recursive).
		func->IsBase = false;
		if (SuperClass == NULL || SuperClass->FindChildFunction(func->Identifier, func->ParameterCount, false, false) == NULL)
			func->IsBase = true;
	
		if (func != NULL && func->IsBase == true && func->Scope == this)
		{
			func->VirtualFunctionTableOffset = VirtualFunctionTableSize++;
		}
	}

	// Copy base methods from all parent classes.
	ClassSymbol* parent = SuperClass;
	if (parent != NULL)
	{
		for (int i = 0 ; i < parent->Children.size(); i++)
		{
			FunctionSymbol* func = dynamic_cast<FunctionSymbol*>(parent->Children.at(i));
			if (func == NULL) continue;

			// Check function dosen't already exist, if it does then assign this func
			// the same virtual function table index.
			FunctionSymbol* ourFunc = dynamic_cast<FunctionSymbol*>(FindChildFunction(func->Identifier, func->ParameterCount, false, false));
			if (ourFunc != NULL)
			{
				ourFunc->VirtualFunctionTableOffset = func->VirtualFunctionTableOffset;
				ourFunc->IsBase = true;
			}
			else if (func != NULL && func->IsBase == true)
			{
				Children.push_back(func);
			}
		}
	}

	// Go through derived classes and build them too.
	for (int i = 0; i < this->DerivedClasses.size(); i++)
	{
		ClassSymbol* child = this->DerivedClasses.at(i);
	
		// Build the virtual childs functions using us as a base.
		child->BuildVirtualFunctions();
	}
}