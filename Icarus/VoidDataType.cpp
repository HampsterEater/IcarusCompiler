// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool VoidDataType::CastTo(DataType* dt, bool explicitCast)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	bool result = false;

	// Check we are not casting to the same thing.
	if (dt->GetPrimitive() == GetPrimitive())
		return true;
	
	PrimitiveDataType pdt = dt->GetPrimitive();
	if (pdt == PDT_CLASS && dynamic_cast<StringDataType*>(dt) != NULL)
		pdt = PDT_STRING;

	switch (pdt)
	{
		case PDT_BOOL: break;
		case PDT_BYTE: break;
		case PDT_SHORT: break;
		case PDT_INT: break;
		case PDT_LONG: break;
		case PDT_FLOAT: break;
		case PDT_DOUBLE: break;
		case PDT_STRING: break;
		case PDT_VOID: break;
		case PDT_CLASS: 
			{
				ClassDataType* classDT = (ClassDataType*)dt;
				break;
			}
	}
	
	// Error? Perhaps this should go in parser?
	if (result == false)
		g_app->EmitError("Unable to cast between '" + GetMnemonicForDataType(this) + "' and '" + GetMnemonicForDataType(dt) + "'.", ES_PARSER, EL_FATAL, parser->GetCurrentToken().Line, parser->GetCurrentToken().Column, parser->GetCurrentToken().Path);

	return false;
}

void VoidDataType::Pop(Register reg)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::PopDestroy()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::Push(Register reg)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::PushLiteral(std::string value)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::PushDefault()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::PushReturn()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::PushVariableValue(VariableSymbol* variable)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::AssignToVariable(VariableSymbol* variable)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::AssignToReturn()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void VoidDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* VoidDataType::OpCompareLess()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpCompareGreater()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpCompareLessEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpCompareGreaterEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpCompareEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpCompareNotEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpBitwiseSHL()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpBitwiseSHR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpBitwiseAND()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpBitwiseOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpBitwiseXOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpBitwiseMOD()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpBitwiseNOT()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpAdd()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpSubtract()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpMultiply()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpDivide()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}
 
DataType* VoidDataType::OpUnaryNeg()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpUnaryNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* VoidDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}