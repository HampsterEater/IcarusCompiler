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
//  Methods.
// --------------------------------------------------------------------------------
std::string RegisterOperand::CreateAssembly()
{
	std::string lit = RegisterMnemonics[(int)OpRegister];

	if (OpDataType != NULL)
	{
		std::string refName = OpDataType->GetAssemblyRefName();
		lit = refName + "[" + lit + "]";
	}

	return lit;
}

std::string IntegerLiteralOperand::CreateAssembly()
{
	return StringHelper::IntToString(Literal);
}

std::string StringLiteralOperand::CreateAssembly()
{
	return Literal;
}

std::string LabelOperand::CreateAssembly()
{
	if (InstructionLabel != NULL)
		return InstructionLabel->UniqueASMName + Postfix;
	else if (SymbolLabel != NULL)
		return SymbolLabel->UniqueASMName + Postfix;
	else
		return VFTableSymbolLabel->UniqueASMName + "_vftable";
}

std::string AddressOperand::CreateAssembly()
{
	std::string asmCode;

	asmCode += SourceType->GetAssemblyRefName();
	asmCode += "[";
	asmCode += Source->CreateAssembly();
	asmCode += "]";

	return asmCode;
}

std::string AddressOffsetOperand::CreateAssembly()
{
	std::string asmCode;

	asmCode += SourceType->GetAssemblyRefName();
	asmCode += "[";
	asmCode += Source->CreateAssembly();
		
	std::string offset = Offset->CreateAssembly();
	if (offset.at(0) != '-') // Add + if not a negative integer.
		asmCode += "+";
	asmCode += offset;
	asmCode += "]";

	return asmCode;
}

bool RegisterOperand::IsEqualTo(Operand* op)
{
	if (op == this)
		return true;
		
	RegisterOperand* realOp = dynamic_cast<RegisterOperand*>(op);
	if (realOp == NULL)
		return false;

	if (realOp->OpRegister == OpRegister &&
		((OpDataType == NULL && realOp->OpDataType == NULL) || realOp->OpDataType->IsEqualTo(OpDataType)))
		return true;

	return false;
}
	
bool IntegerLiteralOperand::IsEqualTo(Operand* op)
{
	if (op == this)
		return true;
		
	IntegerLiteralOperand* realOp = dynamic_cast<IntegerLiteralOperand*>(op);
	if (realOp == NULL)
		return false;

	if (realOp->Literal == Literal)
		return true;

	return false;
}

bool StringLiteralOperand::IsEqualTo(Operand* op)
{
	if (op == this)
		return true;
		
	StringLiteralOperand* realOp = dynamic_cast<StringLiteralOperand*>(op);
	if (realOp == NULL)
		return false;

	if (realOp->Literal == Literal)
		return true;

	return false;
}
	
bool LabelOperand::IsEqualTo(Operand* op)
{
	if (op == this)
		return true;
		
	LabelOperand* realOp = dynamic_cast<LabelOperand*>(op);
	if (realOp == NULL)
		return false;

	if (((InstructionLabel == NULL && realOp->InstructionLabel == NULL) || realOp->InstructionLabel == InstructionLabel) &&
		((SymbolLabel == NULL && realOp->SymbolLabel == NULL) || realOp->SymbolLabel ==  SymbolLabel))
		return true;

	return false;
}

bool AddressOperand::IsEqualTo(Operand* op)
{
	if (op == this)
		return true;
		
	AddressOperand* realOp = dynamic_cast<AddressOperand*>(op);
	if (realOp == NULL)
		return false;

	if (((Source == NULL && realOp->Source == NULL) || realOp->Source == Source) &&
		((SourceType == NULL && realOp->SourceType == NULL) || realOp->SourceType ==  SourceType))
		return true;

	return false;
}

bool AddressOffsetOperand::IsEqualTo(Operand* op)
{
	if (op == this)
		return true;
		
	AddressOffsetOperand* realOp = dynamic_cast<AddressOffsetOperand*>(op);
	if (realOp == NULL)
		return false;

	if (((Source == NULL && realOp->Source == NULL) || realOp->Source == Source) &&
		((SourceType == NULL && realOp->SourceType == NULL) || realOp->SourceType ==  SourceType) &&
		Offset == realOp->Offset)
		return true;

	return false;
}