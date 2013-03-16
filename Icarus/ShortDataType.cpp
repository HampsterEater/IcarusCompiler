// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool ShortDataType::CastTo(DataType* dt, bool explicitCast)
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
		case PDT_INT: 
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

				// Convert from word to dword.
				//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CWD, parser->GetCurrentToken());
				
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
				instruction->AddStringLiteralOperand("_icr__Int_IntToString");
				
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

void ShortDataType::Pop(Register reg)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void ShortDataType::PopDestroy()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);
}

void ShortDataType::Push(Register reg)
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void ShortDataType::PushLiteral(std::string value)
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
	instruction->AddStringLiteralOperand("0x0000FFFF");
				
	// Push result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);

	// Deallocate register.
	parser->DeallocateRegister(reg);
}

void ShortDataType::PushDefault()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("0");
}

void ShortDataType::PushReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
}

void ShortDataType::PushVariableValue(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	// Allocate register.
	Register reg = parser->AllocateRegister(this);
	Register thisReg = parser->AllocateRegister(new IntDataType());

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
	parser->DeallocateRegister(reg);
	parser->DeallocateRegister(thisReg);
}

void ShortDataType::AssignToVariable(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate register.
	Register reg = parser->AllocateRegister(this);
	Register reg16Bit = parser->Register32BitTo16Bit(reg);
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
	instruction->AddRegisterOperand(reg16Bit);

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

void ShortDataType::AssignToReturn()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Pop value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
}

void ShortDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* ShortDataType::OpCompareLess()
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
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETB, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));
	
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return new BoolDataType();
}

DataType* ShortDataType::OpCompareGreater()
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
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETA, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));
	
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return new BoolDataType();
}

DataType* ShortDataType::OpCompareLessEqual()
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
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETBE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));
	
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return new BoolDataType();
}

DataType* ShortDataType::OpCompareGreaterEqual()
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
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETAE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));
	
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return new BoolDataType();
}

DataType* ShortDataType::OpCompareEqual()
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

DataType* ShortDataType::OpCompareNotEqual()
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

DataType* ShortDataType::OpBitwiseSHL()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpBitwiseSHR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpBitwiseAND()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpBitwiseOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpBitwiseXOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpBitwiseMOD()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpBitwiseNOT()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpAdd()
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
	
	// Perform addition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddRegisterOperand(rReg);

	// Truncate.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddStringLiteralOperand("0x0000FFFF");
	
	// Push the result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return this;
}

DataType* ShortDataType::OpSubtract()
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
	
	// Perform subtraction.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SUB, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddRegisterOperand(rReg);

	// Truncate.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddStringLiteralOperand("0x0000FFFF");

	// Push the result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return this;
}

DataType* ShortDataType::OpMultiply()
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
	
	// Perform multiplication.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_IMUL, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddRegisterOperand(rReg);

	// Truncate.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddStringLiteralOperand("0x0000FFFF");
	
	// Push the result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);

	return this;
}

DataType* ShortDataType::OpDivide()
{	
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	parser->SetRegisterAllocation(REG_EDX, true);
	Register lReg = parser->AllocateRegister(this);
	Register rReg = parser->AllocateRegister(this);
	
	// Pop l-value into register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(rReg);

	// Pop r-value into register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Blank out high bits of EDX:EAX register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);
	instruction->AddIntLiteralOperand(0);

	// Move value into low bits of EDX:EAX register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOVZX, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EDX);
	instruction->AddRegisterOperand(lReg);

	// Extend the sign bit from 32 to 64 bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CDQ, parser->GetCurrentToken());
	
	// Perform the divide operation.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_IDIV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(rReg);
	
	// Truncate.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_AND, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddStringLiteralOperand("0x0000FFFF");
	
	// Push the result into the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->DeallocateRegister(rReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	parser->SetRegisterAllocation(REG_EDX, false);

	return this;
}
 
DataType* ShortDataType::OpUnaryNeg()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpUnaryNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* ShortDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}