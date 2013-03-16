// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"

// --------------------------------------------------------------------------------
//  Defines the op code mnemonics.
// --------------------------------------------------------------------------------
std::string OpCodeMnemonics[] = 
{
	"comment",
	"jump target",

	"ret",

	"add",
	"sub",

	"idiv",
	"imul",
	"div",
	"mul",

	"mov",
	"movzx",

	"pop",
	"push",
	"pushd",
	"pushw",

	"jmp",
	"je",
	"jne",
	"cmp",

	"call",

	"setb",
	"seta",
	"setbe",
	"setae",
	"setne",
	"sete",

	"xor",
	"shr",
	"shl",
	"or",
	"not",
	"neg",
	"and",

	"cdq",
	"cwd",
	"cbw",

	"fld",
	"fadd",
	"faddp",
	"fsub",
	"fdiv",
	"fmul",
	"fstp",

	"fucompp",
	"fnstsw",
	"sahf",

	"fcomip",
	"fdecstp",

	"pushad",
	"popad"
};

// --------------------------------------------------------------------------------
//  Defines the register mnemonics.
//  Warning: Always define lowercase! Other code depends on it!
// --------------------------------------------------------------------------------
std::string RegisterMnemonics[] =
{
	"bl",
	"cl",
	"al",
	"dl",
	"ah",
	"bh",
	"ch",
	"dh",

	"bx",
	"cx",
	"ax",
	"dx",
	"sp",
	"bp",
	"si",
	"di",

	"ebx",
	"ecx",
	"eax",
	"edx",
	"esp",
	"ebp",
	"esi",
	"edi",

	"flags",

	"es",
	"cs",
	"ss",
	"ds",
	"fs",
	"gs",

	"cr0",
	"cr2",
	"cr3",
	"cr4",

	"dr0",
	"dr1",
	"dr2",
	"dr3",
	"dr6",
	"dr7",

	"st0",
	"st1",
	"st2",
	"st3",
	"st4",
	"st5",
	"st6",
	"st7",

	"mm0",
	"mm1",
	"mm2",
	"mm3",
	"mm4",
	"mm5",
	"mm6",
	"mm7",

	"xmm0",
	"xmm1",
	"xmm2",
	"xmm3",
	"xmm4",
	"xmm5",
	"xmm6",
	"xmm7",

	"ymm0",
	"ymm1",
	"ymm2",
	"ymm3",
	"ymm4",
	"ymm5",
	"ymm6",
	"ymm7",
};
int RegisterSizes[] =
{
	1,
	1,
	1,
	1,
	1,
	1,
	1,
	1,

	2,
	2,
	2,
	2,
	2,
	2,
	2,
	2,

	4,
	4,
	4,
	4,
	4,
	4,
	4,
	4,

	2,
	2,
	2,
	2,
	2,
	2,

	4,
	4,
	4,
	4,

	4,
	4,
	4,
	4,
	4,
	4,

	10,
	10,
	10,
	10,
	10,
	10,
	10,
	10,

	8,
	8,
	8,
	8,
	8,
	8,
	8,
	8,

	16,
	16,
	16,
	16,
	16,
	16,
	16,
	16,

	32,
	32,
	32,
	32,
	32,
	32,
	32,
	32
};

// --------------------------------------------------------------------------------
//  Methods.
// --------------------------------------------------------------------------------
Instruction::Instruction(Symbol* scope, InstructionOpCode code, Token token)
{
	OpCount = 0;
	OpCode = code;
	Line = token.Line;
	Column = token.Column;
	File = token.Path;
	if (scope != NULL)
		scope->Instructions.push_back(this);
}

// --------------------------------------------------------------------------------
//  Jump Target Methods.
// --------------------------------------------------------------------------------
JumpTarget::JumpTarget(Token token)
{
	Line = token.Line;
	Column = token.Column;
	File = token.Path;
	OpCode = OPCODE_JUMP_TARGET;
}

void JumpTarget::Bind(Symbol* scope)
{
	if (scope != NULL)
		scope->Instructions.push_back(this);
}