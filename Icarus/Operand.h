// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	Foreward declarations of references classes.
// --------------------------------------------------------------------------------
enum Register;
class Instruction;
class Symbol;
class DataType;

// --------------------------------------------------------------------------------
//	This is the base class for instruction operands.
// --------------------------------------------------------------------------------
class Operand
{
//public:
public:

//private:
	virtual std::string CreateAssembly()
	{
		return "";
	}

	virtual bool IsEqualTo(Operand* op)
	{
		if (op == this)
			return true;

		return false;
	}
};

// --------------------------------------------------------------------------------
//	This is the register operand. ESI, EBX, etc
// --------------------------------------------------------------------------------
class RegisterOperand : public Operand
{
//public:
public:
	Register OpRegister;
	DataType* OpDataType;

//private:
	RegisterOperand(Register reg, DataType* type=NULL)
	{
		OpRegister = reg;
		OpDataType = type;
	}

	std::string CreateAssembly();
	bool IsEqualTo(Operand* op);
};

// --------------------------------------------------------------------------------
//	Literal integer operand.
// --------------------------------------------------------------------------------
class IntegerLiteralOperand : public Operand
{
//public:
public:
	int Literal;

//private:
	IntegerLiteralOperand(int lit)
	{
		Literal = lit;
	}

	std::string CreateAssembly();
	bool IsEqualTo(Operand* op);

};

// --------------------------------------------------------------------------------
//	Literal string operand.
// --------------------------------------------------------------------------------
class StringLiteralOperand : public Operand
{
//public:
public:
	std::string Literal;

//private:
	StringLiteralOperand(std::string lit)
	{
		Literal = lit;
	}

	std::string CreateAssembly();
	bool IsEqualTo(Operand* op);

};

// --------------------------------------------------------------------------------
//	This is the label operand. Eg. __literal1, __function1, __jumptarget1, etc
// --------------------------------------------------------------------------------
class LabelOperand : public Operand
{
//public:
public:
	Instruction* InstructionLabel;
	Symbol*		 SymbolLabel;
	Symbol*		 VFTableSymbolLabel;
	std::string  Postfix;

//private:
	LabelOperand(Instruction* instr)
	{
		InstructionLabel = instr;
		SymbolLabel = NULL;
		Postfix = "";
	}
	LabelOperand(Symbol* sym, bool isVF=false)
	{
		InstructionLabel = NULL;
		if (isVF == false)
			SymbolLabel = sym;
		else
			VFTableSymbolLabel = sym;
		Postfix = "";
	}
	LabelOperand(Symbol* sym, std::string post)
	{
		InstructionLabel = NULL;
		SymbolLabel = sym;
		Postfix = post;
	}

	std::string CreateAssembly();
	bool IsEqualTo(Operand* op);

};

// --------------------------------------------------------------------------------
//	This is the indirection operand, it is basically a pointer to a value. 
//  eg. mov eax, dword[eax]
// --------------------------------------------------------------------------------
class AddressOperand : public Operand
{
//public:
public:
	Operand* Source;
	DataType* SourceType;

//private:
	AddressOperand(Operand* source, DataType* type)
	{
		Source = source;
		SourceType = type;
	}

	std::string CreateAssembly();
	bool IsEqualTo(Operand* op);

};

// --------------------------------------------------------------------------------
//	This is the offset indirection operand, it is basically a pointer to a value. 
//  eg. mov eax, dword[eax + 32]
// --------------------------------------------------------------------------------
class AddressOffsetOperand : public Operand
{
//public:
public:
	Operand* Source;
	Operand* Offset;
	DataType* SourceType;

//private:
	AddressOffsetOperand(Operand* source, Operand* offset, DataType* type)
	{
		Source = source;
		Offset = offset;
		SourceType = type;
	}

	std::string CreateAssembly();
	bool IsEqualTo(Operand* op);
};
