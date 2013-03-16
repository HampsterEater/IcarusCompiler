// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool DoubleDataType::CastTo(DataType* dt, bool explicitCast)
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
				instruction->AddStringLiteralOperand("_icr__Double_DoubleToInt");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

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
				instruction->AddStringLiteralOperand("_icr__Double_DoubleToInt");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

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
				instruction->AddStringLiteralOperand("_icr__Double_DoubleToInt");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_LONG: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Double_DoubleToLong");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
			}
		case PDT_FLOAT: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Double_DoubleToFloat");
				
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(8);

				// Push in the resulting value.
				dt->PushReturn();

				result = true;
				break;
				break;
			}
		case PDT_STRING: 
			{
				// Invoke class allocation function.
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Double_DoubleToString");
				
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
		case PDT_CLASS: break;
	}

	// Error? Perhaps this should go in parser?
	if (result == false)
		g_app->EmitError("Unable to cast between '" + GetMnemonicForDataType(this) + "' and '" + GetMnemonicForDataType(dt) + "'.", ES_PARSER, EL_FATAL, parser->GetCurrentToken().Line, parser->GetCurrentToken().Column, parser->GetCurrentToken().Path);

	return result;
}

void DoubleDataType::Pop(Register reg)
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

void DoubleDataType::PopDestroy()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);
}

void DoubleDataType::Push(Register reg)
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

void DoubleDataType::PushLiteral(std::string value)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	double floatLit = (double)strtod(value.c_str(), NULL);
	long long intVersion = *(long long*)(&floatLit);

	int hiBits = (int)((intVersion & 0xFFFFFFFF00000000) >> 32);
	int loBits = (int)(intVersion & 0x00000000FFFFFFFF);
				
	// Push high bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(hiBits);

	// Push low bits.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(loBits);
}

void DoubleDataType::PushDefault()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(0);

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(0);
}

void DoubleDataType::PushReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Push blank value into stack. (hibits)
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(0);

	// Push blank value into stack. (lobits)
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(0);

	// Pop value off FPU stack into our stack space.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType()); 

	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	//instruction->AddRegisterOperand(REG_EAX);
}

void DoubleDataType::PushVariableValue(VariableSymbol* variable)
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
//	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
//	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));

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

void DoubleDataType::AssignToVariable(VariableSymbol* variable)
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

void DoubleDataType::AssignToReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Move value into ST0.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());

	// Pop value out of stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// Pop value into a register.
	//instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	//instruction->AddRegisterOperand(REG_EAX);
}

void DoubleDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

// TODO: All below

DataType* DoubleDataType::OpCompareLess()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(new BoolDataType());

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
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

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
	instruction->AddIntLiteralOperand(16);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* DoubleDataType::OpCompareGreater()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(new BoolDataType());

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
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

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
	instruction->AddIntLiteralOperand(16);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* DoubleDataType::OpCompareLessEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(new BoolDataType());

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
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

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
	instruction->AddIntLiteralOperand(16);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* DoubleDataType::OpCompareGreaterEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(new BoolDataType());

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
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

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
	instruction->AddIntLiteralOperand(16);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* DoubleDataType::OpCompareEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(new BoolDataType());

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
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

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
	instruction->AddIntLiteralOperand(16);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* DoubleDataType::OpCompareNotEqual()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	parser->SetRegisterAllocation(REG_EAX, true);
	Register lReg = parser->AllocateRegister(new BoolDataType());

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
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());

	// Push r-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

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
	instruction->AddIntLiteralOperand(16);
		
	// Push the register result.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(lReg);

	// Deallocate registers.
	parser->DeallocateRegister(lReg);
	parser->SetRegisterAllocation(REG_EAX, false);
	
	return new BoolDataType();
}

DataType* DoubleDataType::OpBitwiseSHL()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpBitwiseSHR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpBitwiseAND()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpBitwiseOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpBitwiseXOR()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpBitwiseMOD()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpBitwiseNOT()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpAdd()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FADD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}

DataType* DoubleDataType::OpSubtract()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSUB, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}

DataType* DoubleDataType::OpMultiply()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FMUL, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}

DataType* DoubleDataType::OpDivide()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate registers.
	Register lReg = parser->AllocateRegister(this);

	// Push l-value floating point value into FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FLD, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Add values from FPU stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FDIV, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 0, new QWordDataType());
	
	// Store float point result into l-value on the stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_FSTP, parser->GetCurrentToken());
	instruction->AddAddressOffsetOperand(REG_ESP, 8, new QWordDataType());

	// Pop r value from stack.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(8);

	// No need to push, value is already on stack.

	// Deallocate registers.
	parser->DeallocateRegister(lReg);

	return this;
}
 
DataType* DoubleDataType::OpUnaryNeg()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpUnaryNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}

DataType* DoubleDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("Error, unimplemented function was called.");
	return NULL;
}