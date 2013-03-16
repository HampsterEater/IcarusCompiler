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
//  Define data arrays.
// --------------------------------------------------------------------------------
std::string PrimitiveDataTypeMnemonics[] = 
{
	"bool",
	"byte",
	"short",
	"int",
	"long",
	"float",
	"double",
	"string",
	"void",
	"User Defined Class"
};

bool DataType::RequiresCast(DataType* dt2)
{
	PrimitiveDataType pdt1 = GetPrimitive();
	if (pdt1 == PDT_CLASS && dynamic_cast<StringDataType*>(this) != NULL)
		pdt1 = PDT_STRING;

	PrimitiveDataType pdt2 = dt2->GetPrimitive();
	if (pdt2 == PDT_CLASS && dynamic_cast<StringDataType*>(dt2) != NULL)
		pdt2 = PDT_STRING;

	return pdt1 != pdt2;
}

// Returns the mnemonic that represents a data type.
std::string DataType::GetMnemonicForDataType(DataType* dt)
{
	if (dt->GetPrimitive() == PDT_CLASS)
	{
		if (((ClassDataType*)dt)->Class != NULL)
			return ((ClassDataType*)dt)->Class->Identifier;
	}
	return PrimitiveDataTypeMnemonics[(int)dt->GetPrimitive()];
}