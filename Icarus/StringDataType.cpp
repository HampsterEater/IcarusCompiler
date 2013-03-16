// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
StringDataType::StringDataType()
{
	Parser* parser = g_app->GetParser();
	Class = (ClassSymbol*)(parser->GetGlobalScope()->FindChild("string"));
}

bool StringDataType::CastTo(DataType* dt, bool explicitCast)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	bool result = false;

	if (dt->GetPrimitive() == GetPrimitive())
		return true;
	
	PrimitiveDataType pdt = dt->GetPrimitive();
	if (pdt == PDT_CLASS && dynamic_cast<StringDataType*>(dt) != NULL)
		pdt = PDT_STRING;

	switch (pdt)
	{
		case PDT_BOOL: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__String_StringToBool");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();
				
				// Truncate.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
				instruction->AddAddressOffsetOperand(REG_ESP, 0, this);
				instruction->AddStringLiteralOperand("0x000000FF");

				result = true;
				break;
			}
		case PDT_BYTE:
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__String_StringToInt");

				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();
				
				// Truncate.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
				instruction->AddAddressOffsetOperand(REG_ESP, 0, this);
				instruction->AddStringLiteralOperand("0x000000FF");

				result = true;
				break;
			}
		case PDT_SHORT: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__String_StringToInt");

				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();
				
				// Truncate.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
				instruction->AddAddressOffsetOperand(REG_ESP, 0, this);
				instruction->AddStringLiteralOperand("0x0000FFFF");

				result = true;
				break;
			}
		case PDT_INT: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__String_StringToInt");
				
				// TODO: TRUNCATE

				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_LONG: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__String_StringToLong");

				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_FLOAT: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__String_StringToFloat");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_DOUBLE:
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__String_StringToDouble");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_VOID: break;
		case PDT_CLASS: break;
	}

	// Error? Perhaps this should go in parser?
	if (result == false)
		g_app->EmitError("Unable to cast between '" + GetMnemonicForDataType(this) + "' and '" + GetMnemonicForDataType(dt) + "'.", ES_PARSER, EL_FATAL, parser->GetCurrentToken().Line, parser->GetCurrentToken().Column, parser->GetCurrentToken().Path);
	
	return result;
}

void StringDataType::PushLiteral(std::string value)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	// Push the label to the string.
	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	//instruction->AddLabelOperand(parser->DefineStringLiteral(value));

	// Invoke the creation function.
	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	//instruction->AddStringLiteralOperand("_icr__String_CreateFromLiteral");

	// Clean up the stack.
	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	//instruction->AddRegisterOperand(REG_ESP);
	//instruction->AddIntLiteralOperand(4);

	// Push the string literal object.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddLabelOperand(parser->DefineStringLiteral(value)); //+ Class_Header_Size

//	instruction->AddIntLiteralOperand(0);
//	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
//	instruction->AddLabelOperand(parser->DefineStringLiteral(value));

	// Push the pointer to the returned object.
	//PushReturn();
}

void StringDataType::PushDefault()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	// Push the label to the string.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddLabelOperand(parser->DefineStringLiteral("", false));

	// Invoke the creation function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__String_CreateFromLiteral");

	// Clean up the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);

	// Push the pointer to the returned object.
	PushReturn();
}

void StringDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* StringDataType::OpCompareLess()
{
	g_app->EmitError("Less than comparison can not be used on boolean data type.");
	return NULL;
}

DataType* StringDataType::OpCompareGreater()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* StringDataType::OpCompareLessEqual()
{
	g_app->EmitError("Less or equal comparison can not be used on boolean data type.");
	return NULL;
}

DataType* StringDataType::OpCompareGreaterEqual()
{
	g_app->EmitError("Greater or equal comparison can not be used on boolean data type.");
	return NULL;
}

DataType* StringDataType::OpCompareEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__String_CEQ");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* StringDataType::OpCompareNotEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__String_CNE");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* StringDataType::OpBitwiseSHL()
{
	g_app->EmitError("Bitwise shift left operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpBitwiseSHR()
{
	g_app->EmitError("Bitwise shift right operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpBitwiseAND()
{
	g_app->EmitError("Bitwise and operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpBitwiseOR()
{
	g_app->EmitError("Bitwise or operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpBitwiseXOR()
{
	g_app->EmitError("Bitwise xor operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpBitwiseMOD()
{
	g_app->EmitError("Bitwise modulus operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpBitwiseNOT()
{	
	g_app->EmitError("Bitwise not operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpAdd()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__String_Concat");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// Push the resulting string.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return this;
}

DataType* StringDataType::OpSubtract()
{	
	g_app->EmitError("Subtract operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpMultiply()
{	
	g_app->EmitError("Multiply operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpDivide()
{	
	g_app->EmitError("Divide operator can not be used on string data type.");
	return NULL;
}
 
DataType* StringDataType::OpUnaryNeg()
{
	g_app->EmitError("Unary negatate operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpUnaryNot()
{
	g_app->EmitError("Unary not operator can not be used on string data type.");
	return NULL;
}

DataType* StringDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Unary logical not operator can not be used on string data type.");
	return NULL;
}