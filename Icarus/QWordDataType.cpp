// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool QWordDataType::CastTo(DataType* dt, bool explicitCast)
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

void QWordDataType::Pop(Register reg)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::PopDestroy()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::Push(Register reg)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::PushLiteral(std::string value)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::PushDefault()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::PushReturn()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::PushVariableValue(VariableSymbol* variable)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::AssignToVariable(VariableSymbol* variable)
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::AssignToReturn()
{
	g_app->EmitError("Error, unimplemented function was called.");
}

void QWordDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* QWordDataType::OpCompareLess()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpCompareGreater()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpCompareLessEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpCompareGreaterEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpCompareEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpCompareNotEqual()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpBitwiseSHL()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpBitwiseSHR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpBitwiseAND()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpBitwiseOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpBitwiseXOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpBitwiseMOD()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpBitwiseNOT()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpAdd()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpSubtract()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpMultiply()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpDivide()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}
 
DataType* QWordDataType::OpUnaryNeg()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpUnaryNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* QWordDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}