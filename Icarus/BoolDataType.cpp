// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool BoolDataType::CastTo(DataType* dt, bool explicitCast)
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
		case PDT_BYTE: 
			{
				// Same Size
				result = true;
				break;
			}
		case PDT_INT: 
		case PDT_SHORT:
			{
				// Allocate register.
				Register reg = parser->AllocateRegister(this);
				Register reg2Main = parser->AllocateRegister(this);
				Register reg2 = parser->Register32BitTo8Bit(reg2Main);

				// Pop value into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg2);

				// Truncate.
				//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
				//instruction->AddRegisterOperand(reg);
				//instruction->AddStringLiteralOperand("0x000000FF");
				
				// Move with zero extend.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOVZX, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg);
				instruction->AddRegisterOperand(reg2);

				// TODO: CBW, CWD

				// Push into stack.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg);
							
				// Deallocate register.
				parser->DeallocateRegister(reg2Main);
				parser->DeallocateRegister(reg);

				result = true;
				break;
			}

		case PDT_LONG:
			{
				// Allocate register.				
				Register reg = REG_EAX;
				parser->SetRegisterAllocation(REG_EAX, true);
				parser->SetRegisterAllocation(REG_EDX, true);
				//Register reg = parser->AllocateRegister(dt);
				Register loBits = parser->Register64LoBitsTwin(reg);
				Register hiBits = parser->Register64HiBitsTwin(reg);

				// Pop value into a register.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
				instruction->AddRegisterOperand(reg);
				
				// Truncate.
				//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
				//instruction->AddRegisterOperand(reg);
				//instruction->AddStringLiteralOperand("0x0000FFFF");
				
				// Convert from byte to word.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CBW, parser->GetCurrentToken());
				
				// Convert from word to dword.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CWD, parser->GetCurrentToken());
				
				// Convert from dword to quad.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CDQ, parser->GetCurrentToken());
				
				// Push hi-bits onto stack.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
				instruction->AddRegisterOperand(hiBits);
				
				// Push lo-bits onto stack.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
				instruction->AddRegisterOperand(loBits);
			
				// Deallocate register.
				parser->SetRegisterAllocation(REG_EAX, false);
				parser->SetRegisterAllocation(REG_EDX, false);

				result = true;
				break;
			}
		case PDT_FLOAT: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Int_IntToFloat");
				
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
				instruction->AddStringLiteralOperand("_icr__Int_IntToDouble");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_STRING: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Bool_BoolToString");
				
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

void BoolDataType::Pop(Register reg)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void BoolDataType::PopDestroy()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);
}

void BoolDataType::Push(Register reg)
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void BoolDataType::PushLiteral(std::string value)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// TODO: Physically truncate use min/max
	
	// Allocate register.
	Register reg = parser->AllocateRegister(this);

	// Move into register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
	instruction->AddStringLiteralOperand(value);

	// Truncate.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
	instruction->AddStringLiteralOperand("0x000000FF");
				
	// Push result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);

	// Deallocate register.
	parser->DeallocateRegister(reg);
}

void BoolDataType::PushDefault()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("0");
}

void BoolDataType::PushReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
}

void BoolDataType::PushVariableValue(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	// Allocate register.
	Register thisReg = parser->AllocateRegister(new IntDataType());
	Register reg = parser->AllocateRegister(this);

	// Pop the pointer to the class off the stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Move into register so we can pad to 32bit.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOVZX, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));

	// Push "this" variable back into stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Push register onto the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);

	// Deallocate register.
	parser->DeallocateRegister(thisReg);
	parser->DeallocateRegister(reg);
}

void BoolDataType::AssignToVariable(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate register.
	Register reg = parser->AllocateRegister(this);
	Register reg8Bit = parser->Register32BitTo8Bit(reg);
	Register thisReg = parser->AllocateRegister(new IntDataType());

	// Pop value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
		
	// Pop the pointer to the class off the stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Assign value to variable.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));
	instruction->AddRegisterOperand(reg8Bit);
	
	// Push "this" variable back into stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Deallocate register.
	parser->DeallocateRegister(reg);
	parser->DeallocateRegister(thisReg);
}

void BoolDataType::AssignToReturn()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Pop value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
}

void BoolDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* BoolDataType::OpCompareLess()
{
	g_app->EmitError("Less than comparison can not be used on boolean data type.");
	return this;
}

DataType* BoolDataType::OpCompareGreater()
{
	g_app->EmitError("Greater than comparison can not be used on boolean data type.");
	return this;
}

DataType* BoolDataType::OpCompareLessEqual()
{
	g_app->EmitError("Less or equal comparison can not be used on boolean data type.");
	return this;
}

DataType* BoolDataType::OpCompareGreaterEqual()
{
	g_app->EmitError("Greater or equal comparison can not be used on boolean data type.");
	return this;
}

DataType* BoolDataType::OpCompareEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);
	Register rReg = parser->AllocateRegister(this);

	// Pop l-value into register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(rReg);

	// Pop r-value into register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Perform comparison.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CMP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddRegisterOperand(rReg);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));
	
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return new BoolDataType();
}

DataType* BoolDataType::OpCompareNotEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);
	Register rReg = parser->AllocateRegister(this);

	// Pop l-value into register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(rReg);

	// Pop r-value into register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Perform comparison.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CMP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddRegisterOperand(rReg);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETNE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));
	
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return new BoolDataType();
}

DataType* BoolDataType::OpBitwiseSHL()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpBitwiseSHR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpBitwiseAND()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpBitwiseOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpBitwiseXOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpBitwiseMOD()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpBitwiseNOT()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpAdd()
{
	g_app->EmitError("Addition operator can not be used on boolean data type.");
	return this;
}

DataType* BoolDataType::OpSubtract()
{
	g_app->EmitError("Subtraction operator can not be used on boolean data type.");
	return this;
}

DataType* BoolDataType::OpMultiply()
{
	g_app->EmitError("Multiplication operator can not be used on boolean data type.");
	return this;
}

DataType* BoolDataType::OpDivide()
{	
	g_app->EmitError("Division operator can not be used on boolean data type.");
	return this;
}
 
DataType* BoolDataType::OpUnaryNeg()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpUnaryNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* BoolDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}