// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"

void VariableSymbol::BuildClassHierarchy()
{
	// Patch return-type if its an object.
	ClassDataType* classType = dynamic_cast<ClassDataType*>(DataType);
	if (classType != NULL && classType->Class == NULL)
		classType->Class = dynamic_cast<ClassSymbol*>(Scope->FindChild(classType->ClassName));
}
