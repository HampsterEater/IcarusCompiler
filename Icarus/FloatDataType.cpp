// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool FloatDataType::CastTo(DataType* dt, bool explicitCast)
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
		case PDT_BYTE: 
		case PDT_BOOL: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Float_FloatToInt");
				
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
				instruction->AddStringLiteralOperand("_icr__Float_FloatToInt");
				
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
				instruction->AddStringLiteralOperand("_icr__Float_FloatToInt");
				
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
				instruction->AddStringLiteralOperand("_icr__Float_FloatToLong");
				
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
				instruction->AddStringLiteralOperand("_icr__Float_FloatToDouble");
				
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
				instruction->AddStringLiteralOperand("_icr__Float_FloatToString");
				
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

void FloatDataType::Pop(Register reg)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void FloatDataType::PopDestroy()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);
}

void FloatDataType::Push(Register reg)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void FloatDataType::PushLiteral(std::string value)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	float floatLit = (float)strtod(value.c_str(), NULL);
	int intVersion = *(int*)(&floatLit);

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(intVersion);
}

void FloatDataType::PushDefault()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(0);
}

void FloatDataType::PushReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Push black value into stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(0);

	// Pop value off FPU stack into our stack space.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	//instruction->AddRegisterOperand(REG_EAX);
}

void FloatDataType::PushVariableValue(VariableSymbol* variable)
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
	
	// Push register onto the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));

	// Deallocate register.
	parser->DeallocateRegister(thisReg);
}

void FloatDataType::AssignToVariable(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate register.
	Register reg = parser->AllocateRegister(this);
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
	instruction->AddRegisterOperand(reg);
			
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

void FloatDataType::AssignToReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Move value into ST0.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	// Pop value out of stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);

	// Pop value into a register.
	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	//instruction->AddRegisterOperand(REG_EAX);
}

void FloatDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* FloatDataType::OpCompareLess()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(this);

	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
	instruction->AddIntLiteralOperand(0);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Compare values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FUCOMPP, parser->GetCurrentToken());
	
	// Copy flags into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FNSTSW, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_AX);
	
	// Copy flags from register to FLAGS bitmask.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SAHF, parser->GetCurrentToken());

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETB, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));

	// Pop r/l value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* FloatDataType::OpCompareGreater()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(this);

	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
	instruction->AddIntLiteralOperand(0);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Compare values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FUCOMPP, parser->GetCurrentToken());
	
	// Copy flags into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FNSTSW, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_AX);
	
	// Copy flags from register to FLAGS bitmask.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SAHF, parser->GetCurrentToken());

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETA, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));

	// Pop r/l value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* FloatDataType::OpCompareLessEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(this);

	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
	instruction->AddIntLiteralOperand(0);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Compare values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FUCOMPP, parser->GetCurrentToken());
	
	// Copy flags into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FNSTSW, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_AX);
	
	// Copy flags from register to FLAGS bitmask.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SAHF, parser->GetCurrentToken());

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETBE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));

	// Pop r/l value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* FloatDataType::OpCompareGreaterEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(this);

	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
	instruction->AddIntLiteralOperand(0);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Compare values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FUCOMPP, parser->GetCurrentToken());
	
	// Copy flags into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FNSTSW, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_AX);
	
	// Copy flags from register to FLAGS bitmask.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SAHF, parser->GetCurrentToken());

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETAE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));

	// Pop r/l value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* FloatDataType::OpCompareEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(this);

	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
	instruction->AddIntLiteralOperand(0);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Compare values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FUCOMPP, parser->GetCurrentToken());
	
	// Copy flags into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FNSTSW, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_AX);
	
	// Copy flags from register to FLAGS bitmask.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SAHF, parser->GetCurrentToken());

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));

	// Pop r/l value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* FloatDataType::OpCompareNotEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(this);

	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);
	instruction->AddIntLiteralOperand(0);
	
	// Zero out register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_MOV, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
	instruction->AddIntLiteralOperand(0);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Compare values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FUCOMPP, parser->GetCurrentToken());
	
	// Copy flags into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FNSTSW, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_AX);
	
	// Copy flags from register to FLAGS bitmask.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SAHF, parser->GetCurrentToken());

	// Set register depending on condition.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_SETNE, parser->GetCurrentToken());
	instruction->AddRegisterOperand(parser->Register32BitTo8Bit(lReg));

	// Pop r/l value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* FloatDataType::OpBitwiseSHL()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpBitwiseSHR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpBitwiseAND()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpBitwiseOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpBitwiseXOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpBitwiseMOD()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpBitwiseNOT()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpAdd()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FADD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}

DataType* FloatDataType::OpSubtract()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSUB, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}

DataType* FloatDataType::OpMultiply()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FMUL, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}

DataType* FloatDataType::OpDivide()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FDIV, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, this);
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 4, this);

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}
 
DataType* FloatDataType::OpUnaryNeg()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpUnaryNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* FloatDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}