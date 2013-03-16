// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"
	
bool ClassDataType::CastTo(DataType* dt, bool explicitCast)
{	
//	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	bool result = false;

	// Check we are not casting to the same thing. // Not valid for classes!
	//if (dt->GetPrimitive() == GetPrimitive())
	//	return true;
	
	// String class is a special case.
	PrimitiveDataType pdt = dt->GetPrimitive();
	if (pdt == PDT_CLASS && dynamic_cast<StringDataType*>(dt) != NULL)
		pdt = PDT_STRING;

	switch (pdt)
	{
		case PDT_BOOL: break;
		case PDT_BYTE: break;
		case PDT_SHORT: break;
		case PDT_INT: 
			{
				result = true; // Same size.
				break;
			}
		case PDT_LONG: break;
		case PDT_FLOAT: break;
		case PDT_DOUBLE: break;
		case PDT_STRING: 
			{
				// Invoke the increment reference function.
				Instruction* instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
				instruction->AddStringLiteralOperand("_icr__Class_ToString");
					
				// Cleanup Stack
				instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);
					
				// Push in the resulting value.
				PushReturn();

				result = true; 

				break;
			}
		case PDT_VOID: break;
		case PDT_CLASS:
			{
				ClassDataType* sourceClassDT = this;
				ClassDataType* destClassDT = (ClassDataType*)dt;

				// Check src super classes for base class
				// if exists, ok.
				// 
				// BaseClass var = SubClass();
				// SupClass var = BaseClass();

				// Check for casting to super classes.
				bool validCast = false;
				
				ClassSymbol* superClass = sourceClassDT->Class;
				while (superClass != NULL)
				{
					if (superClass == destClassDT->Class)
					{
						validCast = true;
						break;
					}
					superClass = superClass->SuperClass;
				}

				if (validCast == true)
					result = true;
				
				// Do we need to do an implicit cast?
				else if (explicitCast == true)
				{
					// Push in the class index of whatever we want to cast to.
					Instruction* instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
					instruction->AddIntLiteralOperand(destClassDT->Class->ClassDefinitionIndex);

					// Invoke the increment reference function.
					instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
					instruction->AddStringLiteralOperand("_icr__ExplicitCast");

					// Cleanup Stack
					instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
					instruction->AddRegisterOperand(REG_ESP);
					instruction->AddIntLiteralOperand(8);
					
					// Push in the resulting value.
					dt->PushReturn();

					result = true;
				}
	
				break;
			}
	}

	// Error? Perhaps this should go in parser?
	if (result == false)
		g_app->EmitError("Unable to cast between '" + GetMnemonicForDataType(this) + "' and '" + GetMnemonicForDataType(dt) + "'.", ES_PARSER, EL_FATAL, parser->GetCurrentToken().Line, parser->GetCurrentToken().Column, parser->GetCurrentToken().Path);

	return false;
}

void ClassDataType::Pop(Register reg)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void ClassDataType::PopDestroy()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();
	
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);
}

void ClassDataType::Push(Register reg)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
}

void ClassDataType::PushLiteral(std::string value)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand(value);
}

void ClassDataType::PushDefault()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddIntLiteralOperand(0);
}

void ClassDataType::PushReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSHD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
}

void ClassDataType::PushVariableValue(VariableSymbol* variable)
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

void ClassDataType::AssignToVariable(VariableSymbol* variable)
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Allocate register.
	Register reg = parser->AllocateRegister(this);
	Register thisReg = parser->AllocateRegister(new IntDataType());

	// Pop value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(reg);
	
	// Decrement reference for variable for old object.
	DecrementVariableRef(variable);

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

	// Increment reference for variable for new object.
	IncrementVariableRef(variable);

	// Deallocate register.
	parser->DeallocateRegister(reg);
	parser->DeallocateRegister(thisReg);
}

void ClassDataType::IncrementVariableRef(VariableSymbol* variable)
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

	PushReturn();

	// Get the pointer to the variables value.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));

	// Invoke the increment reference function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__IncrementRefCount");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);
			
	AssignToReturn();

	// Push "this" variable back into stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Deallocate register.
	parser->DeallocateRegister(thisReg);	
}

void ClassDataType::DecrementVariableRef(VariableSymbol* variable)
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
	
	PushReturn();

	// Get the pointer to the variables value.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
	instruction->AddOperand(parser->GetVariableAccessOperand(variable, 0, thisReg));

	// Invoke the increment reference function.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_CALL, parser->GetCurrentToken());
	instruction->AddStringLiteralOperand("_icr__DecrementRefCount");

	// Cleanup Stack
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_ADD, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_ESP);
	instruction->AddIntLiteralOperand(4);
	
	AssignToReturn();
			
	// Push "this" variable back into stack.
	if (variable->VariableType == VT_FIELD)
	{
		instruction = new Instruction(parser->GetInstructionScope(), OPCODE_PUSH, parser->GetCurrentToken());
		instruction->AddRegisterOperand(thisReg);
	}

	// Deallocate register.
	parser->DeallocateRegister(thisReg);	
}

void ClassDataType::AssignToReturn()
{
	Instruction* instruction;
	Parser* parser = g_app->GetParser();

	// Pop value into a register.
	instruction = new Instruction(parser->GetInstructionScope(), OPCODE_POP, parser->GetCurrentToken());
	instruction->AddRegisterOperand(REG_EAX);
}

void ClassDataType::NullifyVariable(VariableSymbol* variable)
{
	PushDefault();
	AssignToVariable(variable);
}

DataType* ClassDataType::OpCompareLess()
{
	g_app->EmitError("The less than comparison operator can not be used on class data type's.");
	return NULL;
}

DataType* ClassDataType::OpCompareGreater()
{
	g_app->EmitError("The greater than comparison operator can not be used on class data type's.");
	return NULL;
}

DataType* ClassDataType::OpCompareLessEqual()
{
	g_app->EmitError("The less or equal comparison operator can not be used on class data type's.");
	return NULL;
}

DataType* ClassDataType::OpCompareGreaterEqual()
{
	g_app->EmitError("The greater or equal comparison operator can not be used on class data type's.");
	return NULL;
}

DataType* ClassDataType::OpCompareEqual()
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

DataType* ClassDataType::OpCompareNotEqual()
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

DataType* ClassDataType::OpBitwiseSHL()
{
	g_app->EmitError("The bitwise shift left operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpBitwiseSHR()
{
	g_app->EmitError("The bitwise shift right operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpBitwiseAND()
{
	g_app->EmitError("The bitwise and operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpBitwiseOR()
{
	g_app->EmitError("The bitwise or operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpBitwiseXOR()
{
	g_app->EmitError("The bitwise xor operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpBitwiseMOD()
{
	g_app->EmitError("The bitwise modulus operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpBitwiseNOT()
{
	g_app->EmitError("The bitwise not operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpAdd()
{
	g_app->EmitError("The add operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpSubtract()
{
	g_app->EmitError("The subtract operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpMultiply()
{
	g_app->EmitError("The multiply operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpDivide()
{
	g_app->EmitError("The divide operator can not be used on class data type's.");
	return this;
}
 
DataType* ClassDataType::OpUnaryNeg()
{
	g_app->EmitError("The unary negatate operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpUnaryNot()
{
	g_app->EmitError("The unary not operator can not be used on class data type's.");
	return this;
}

DataType* ClassDataType::OpUnaryLogicalNot()
{
	g_app->EmitError("The unary logical not operator can not be used on class data type's.");
	return this;
}