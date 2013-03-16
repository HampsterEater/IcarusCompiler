// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool LongDataType::CastTo(DataType* dt, bool explicitCast)
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
		case PDT_BOOL: 
		case PDT_BYTE: 
			{	
				// Allocate register.
				Register mainReg = parser->AllocateRegister(this);
				Register reg1 = parser->Register64LoBitsTwin(mainReg);
				Register reg2 = parser->Register64HiBitsTwin(mainReg);
	
				// Pop lobits into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1);
				
				// Pop hibits into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg2);
				
				// Truncate
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1);
				instruction->AddStringLiteralOperand("0x000000FF");

				// Push low bits into stack.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1); 

				// Deallocate register.
				parser->DeallocateRegister(mainReg);

				result = true;
				break;
			}
		case PDT_SHORT:
			{
				// Allocate register.
				Register mainReg = parser->AllocateRegister(this);
				Register reg1 = parser->Register64LoBitsTwin(mainReg);
				Register reg2 = parser->Register64HiBitsTwin(mainReg);
	
				// Pop lobits into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1);
				
				// Pop hibits into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg2);
				
				// Truncate
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1);
				instruction->AddStringLiteralOperand("0x0000FFFF");

				// Push low bits into stack.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1); 
			
				// Deallocate register.
				parser->DeallocateRegister(mainReg);

				result = true;
				break;
			}
		case PDT_INT: 
			{
				// Allocate register.
				Register mainReg = parser->AllocateRegister(this);
				Register reg1 = parser->Register64LoBitsTwin(mainReg);
				Register reg2 = parser->Register64HiBitsTwin(mainReg);
	
				// Pop lobits into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1);
				
				// Pop hibits into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg2);

				// Push low bits into stack.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg1); 

				// Deallocate register.
				parser->DeallocateRegister(mainReg);

				result = true;
				break;
			}
		case PDT_FLOAT: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Long_LongToFloat");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_DOUBLE:
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Long_LongToDouble");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_STRING:
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Long_LongToString");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_VOID: break;
		case PDT_CLASS: 
			{
				ClassDataType* classDT = (ClassDataType*)dt;
				result = false;
				break;
			}
	}

	// Error? Perhaps this should go in parser?
	if (result == false)
		g_app->EmitError("Unable to cast between '" + GetMnemonicForDataType(this) + "' and '" + GetMnemonicForDataType(dt) + "'.", ES_PARSER, EL_FATAL, parser->GetCurrentToken().Line, parser->GetCurrentToken().Column, parser->GetCurrentToken().Path);
	
	return result;
}

void LongDataType::Pop(Register reg)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	Register hiBitsReg = parser->Register64HiBitsTwin(reg);
	Register loBitsReg = parser->Register64LoBitsTwin(reg);

	// Pop out low-bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(loBitsReg); 

	// Pop out high-bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(hiBitsReg); 
}

void LongDataType::PopDestroy()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
}

void LongDataType::Push(Register reg)
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	Register hiBitsReg = parser->Register64HiBitsTwin(reg);
	Register loBitsReg = parser->Register64LoBitsTwin(reg);

	// Push in hi-bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(hiBitsReg);
	
	// Push in low-bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(loBitsReg); 
}

void LongDataType::PushLiteral(std::string value)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	signed long long longVal = StringHelper::StringToLong(value);

	// TODO: Physically truncate use min/max
	
	// Allocate register.
	//Register reg1 = REG_EAX;
	//Register reg2 = REG_EDX;
	//parser->SetRegisterAllocation(REG_EAX, true);
	//parser->SetRegisterAllocation(REG_EDX, true);

	// Move into register.
	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	//instruction->AddRegisterOperand(reg1);
	//instruction->AddStringLiteralOperand("dword[" + value + "]");

	int hiBits = (int)((longVal & 0xFFFFFFFF00000000) >> 32);
	int loBits = (int)(longVal & 0x00000000FFFFFFFF);
				
	// Push high bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(hiBits);

	// Push low bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(loBits);
	
	// Deallocate register.
	//parser->DeallocateRegister(reg1);
	//parser->DeallocateRegister(reg2);
}

void LongDataType::PushDefault()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("dword[0]");
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("dword[0]");
}

void LongDataType::PushReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
}

void LongDataType::PushVariableValue(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	// Allocate register.
	Register thisReg = parser->AllocateRegister(new IntDataType());

	// Pop the pointer to the class off the stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Push "this" variable back into stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Push high-bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));

	// Push low-bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 
															 variable->VariableType == VT_PARAMETER ? -4 : 4, 
															 thisReg));
	// Deallocate register.
	parser->DeallocateRegister(thisReg);
}

void LongDataType::AssignToVariable(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate register.
	Register mainReg = parser->AllocateRegister(this);
	Register reg1 = parser->Register64LoBitsTwin(mainReg);
	Register reg2 = parser->Register64HiBitsTwin(mainReg);
	Register thisReg = parser->AllocateRegister(new IntDataType());

	// Pop lobits value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg1);

	// Pop hibits value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg2);

	// Pop the pointer to the class off the stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Assign highbits value to variable.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));
	instruction->AddRegisterOperand(reg2);

	// Assign lowbits value to variable.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 
															 variable->VariableType == VT_PARAMETER ? -4 : 4, 
															 thisReg));
	instruction->AddRegisterOperand(reg1);

	// Push "this" variable back into stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Deallocate register.
	parser->DeallocateRegister(mainReg);
	parser->DeallocateRegister(thisReg);
}

void LongDataType::AssignToReturn()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Pop lowbits value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	// Pop highbits value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);
}

void LongDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* LongDataType::OpCompareLess()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongCLT");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* LongDataType::OpCompareGreater()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongCGT");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* LongDataType::OpCompareLessEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongCLE");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* LongDataType::OpCompareGreaterEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongCGE");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* LongDataType::OpCompareEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongCEQ");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* LongDataType::OpCompareNotEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongCNE");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return new BoolDataType();
}

DataType* LongDataType::OpBitwiseSHL()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpBitwiseSHR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpBitwiseAND()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpBitwiseOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpBitwiseXOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpBitwiseMOD()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpBitwiseNOT()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpAdd()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongAdd");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the hi bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);

	// Push the lo bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return this;
}

DataType* LongDataType::OpSubtract()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongSub");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the hi bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);

	// Push the lo bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return this;
}

DataType* LongDataType::OpMultiply()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongMul");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the hi bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);

	// Push the lo bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return this;
}

DataType* LongDataType::OpDivide()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Invoke function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__LongDiv");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(16);

	// Push the hi bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);

	// Push the lo bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	return this;
}
 
DataType* LongDataType::OpUnaryNeg()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpUnaryNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* LongDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}