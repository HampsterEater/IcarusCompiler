// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	Foreward declarations.
// --------------------------------------------------------------------------------
class JumpTarget;

// --------------------------------------------------------------------------------
//	These are the different opcodes and instruction can use.
// --------------------------------------------------------------------------------
enum InstructionOpCode
{
	OPCODE_COMMENT = 0,
	OPCODE_JUMP_TARGET,

	OPCODE_RETURN,

	OPCODE_ADD,
	OPCODE_SUB,

	OPCODE_IDIV,
	OPCODE_IMUL,
	OPCODE_DIV,
	OPCODE_MUL,

	OPCODE_MOV,
	OPCODE_MOVZX,

	OPCODE_POP,
	OPCODE_PUSH,
	OPCODE_PUSHD,
	OPCODE_PUSHW,

	OPCODE_JMP,
	OPCODE_JE,
	OPCODE_JNE,
	OPCODE_CMP,

	OPCODE_CALL,

	OPCODE_SETB,
	OPCODE_SETA,
	OPCODE_SETBE,
	OPCODE_SETAE,
	OPCODE_SETNE,
	OPCODE_SETE,

	OPCODE_XOR,
	OPCODE_SHR,
	OPCODE_SHL,
	OPCODE_OR,
	OPCODE_NOT,
	OPCODE_NEG,
	OPCODE_AND,
	
	OPCODE_CDQ,
	OPCODE_CWD,
	OPCODE_CBW,

	OPCODE_FLD,
	OPCODE_FADD,
	OPCODE_FADDP,
	OPCODE_FSUB,
	OPCODE_FDIV,
	OPCODE_FMUL,
	OPCODE_FSTP,

	OPCODE_FUCOMPP,
	OPCODE_FNSTSW,
	OPCODE_SAHF,

	OPCODE_FCOMIP,
	OPCODE_FDECSTP,

	OPCODE_PUSHAD,
	OPCODE_POPAD,
};
extern std::string OpCodeMnemonics[];

// --------------------------------------------------------------------------------
//	These are the different registers that can be used.
// --------------------------------------------------------------------------------
enum Register
{
	REG_BL = 0,
	REG_CL,
	REG_AL,
	REG_DL,
	REG_AH,
	REG_BH,
	REG_CH,
	REG_DH,

	REG_BX,
	REG_CX,
	REG_AX,
	REG_DX,
	REG_SP,
	REG_BP,
	REG_SI,
	REG_DI,

	REG_EBX,
	REG_ECX,
	REG_EAX,
	REG_EDX,
	REG_ESP,
	REG_EBP,
	REG_ESI,
	REG_EDI,

	REG_FLAGS,

	REG_ES,
	REG_CS,
	REG_SS,
	REG_DS,
	REG_FS,
	REG_GS,

	REG_CR0,
	REG_CR2,
	REG_CR3,
	REG_CR4,

	REG_DR0,
	REG_DR1,
	REG_DR2,
	REG_DR3,
	REG_DR6,
	REG_DR7,

	REG_ST0,
	REG_ST1,
	REG_ST2,
	REG_ST3,
	REG_ST4,
	REG_ST5,
	REG_ST6,
	REG_ST7,

	REG_MM0,
	REG_MM1,
	REG_MM2,
	REG_MM3,
	REG_MM4,
	REG_MM5,
	REG_MM6,
	REG_MM7,

	REG_XMM0,
	REG_XMM1,
	REG_XMM2,
	REG_XMM3,
	REG_XMM4,
	REG_XMM5,
	REG_XMM6,
	REG_XMM7,

	REG_YMM0,
	REG_YMM1,
	REG_YMM2,
	REG_YMM3,
	REG_YMM4,
	REG_YMM5,
	REG_YMM6,
	REG_YMM7,
};
extern std::string RegisterMnemonics[];
extern int RegisterSizes[];
#define REGISTER_COUNT 73

// --------------------------------------------------------------------------------
//	This is the base code class for an instruction. It describes
//  an abstract instruction that is turned into machine instructions
//  later by the code generator.
// --------------------------------------------------------------------------------
class Instruction
{
public:
//private:
	InstructionOpCode OpCode;
	Operand* Operands[3];
	int OpCount;

	int Line, Column;
	std::string File;

	std::string UniqueASMName;

//public:
	Instruction() {}
	Instruction(Symbol* scope, InstructionOpCode code, Token token);
	
	void AddOperand(Operand* op)
	{
		Operands[OpCount++] = op;
	}

	void AddRegisterOperand(Register reg, DataType* type=NULL)
	{
		Operands[OpCount++] = new RegisterOperand(reg, type);
	}

	void AddLabelOperand(Instruction* instr)
	{
		Operands[OpCount++] = new LabelOperand(instr);
	}
	void AddLabelOperand(Symbol* sym)
	{
		Operands[OpCount++] = new LabelOperand(sym);
	}
	void AddLabelOperand(Symbol* sym, std::string str)
	{
		Operands[OpCount++] = new LabelOperand(sym, str);
	}
	void AddLabelOperand(JumpTarget* sym)
	{
		Operands[OpCount++] = new LabelOperand((Instruction*)sym);
	}

	void AddIntLiteralOperand(int lit)
	{
		Operands[OpCount++] = new IntegerLiteralOperand(lit);
	}
	void AddStringLiteralOperand(std::string lit)
	{
		Operands[OpCount++] = new StringLiteralOperand(lit);
	}
	
	void AddAddressOperand(Operand* source, DataType* type)
	{
		Operands[OpCount++] = new AddressOperand(source, type);
	}
	void AddAddressOperand(Register source, DataType* type)
	{
		Operands[OpCount++] = new AddressOperand(new RegisterOperand(source), type);
	}
	void AddAddressOperand(Instruction* label, DataType* type)
	{
		Operands[OpCount++] = new AddressOperand(new LabelOperand(label), type);
	}
	void AddAddressOperand(Symbol* label, DataType* type)
	{
		Operands[OpCount++] = new AddressOperand(new LabelOperand(label), type);
	}

	void AddAddressOffsetOperand(Operand* source, Operand* offset, DataType* type)
	{
		Operands[OpCount++] = new AddressOffsetOperand(source, offset, type);
	}
	void AddAddressOffsetOperand(Register source, int offset, DataType* type)
	{
		Operands[OpCount++] = new AddressOffsetOperand(new RegisterOperand(source), new IntegerLiteralOperand(offset), type);
	}
	void AddAddressOffsetOperand(Register source, Register offset, DataType* type)
	{
		Operands[OpCount++] = new AddressOffsetOperand(new RegisterOperand(source), new RegisterOperand(offset), type);
	}
	void AddAddressOffsetOperand(Instruction* source, int offset, DataType* type)
	{
		Operands[OpCount++] = new AddressOffsetOperand(new LabelOperand(source), new IntegerLiteralOperand(offset), type);
	}
	void AddAddressOffsetOperand(Instruction* source, Register offset, DataType* type)
	{
		Operands[OpCount++] = new AddressOffsetOperand(new LabelOperand(source), new RegisterOperand(offset), type);
	}
	void AddAddressOffsetOperand(Symbol* source, int offset, DataType* type)
	{
		Operands[OpCount++] = new AddressOffsetOperand(new LabelOperand(source), new IntegerLiteralOperand(offset), type);
	}
	void AddAddressOffsetOperand(Symbol* source, Register offset, DataType* type)
	{
		Operands[OpCount++] = new AddressOffsetOperand(new LabelOperand(source), new RegisterOperand(offset), type);
	}

};

// --------------------------------------------------------------------------------
//	This is an extension of the instruction class that is used to define a jump
//  target label.
// --------------------------------------------------------------------------------
class JumpTarget : Instruction
{
public:
//private:
	
//public:
	JumpTarget(Token token);
	void Bind(Symbol* scope);

};