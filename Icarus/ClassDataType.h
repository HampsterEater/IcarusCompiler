// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "DataType.h"

class ClassSymbol;

class ClassDataType : public DataType
{
//public:
	//ClassDataType();

public:
	ClassSymbol* Class;
	std::string ClassName;
	
	bool DoNotUseVirutal;

	ClassDataType()
	{
		Class = NULL;
		DoNotUseVirutal = false;
	}
	ClassDataType(ClassSymbol* symbol)
	{
		Class = symbol;
		DoNotUseVirutal = false;
	}

	PrimitiveDataType GetPrimitive()
	{
		return PDT_CLASS;
	}
	std::string GetAssemblyRefName()
	{
		return "dword";
	}
	std::string GetAssemblyDefName()
	{
		return "dd";
	}
	int GetStoredSize()
	{
		return 4;
	}
	int GetSize()
	{
		return 4;
	}
	bool PromoteToParameter()
	{
		return false;
	}
	int GetImplicityCastPriority()
	{
		return -1;
	}
	
	bool CastTo(DataType* dt, bool explicitCast=false);

	void Pop(Register reg);
	void PopDestroy();
	void Push(Register reg);
	void PushLiteral(std::string value);
	void PushDefault();
	void PushReturn();
	void PushVariableValue(VariableSymbol* variable);
	void AssignToVariable(VariableSymbol* variable);
	void AssignToReturn();
	void NullifyVariable(VariableSymbol* variable);

	void IncrementVariableRef(VariableSymbol* variable);
	void DecrementVariableRef(VariableSymbol* variable);

	DataType* OpCompareLess();
	DataType* OpCompareGreater();
	DataType* OpCompareLessEqual();
	DataType* OpCompareGreaterEqual();
	DataType* OpCompareEqual();
	DataType* OpCompareNotEqual();
	DataType* OpBitwiseSHL();
	DataType* OpBitwiseSHR();
	DataType* OpBitwiseAND();
	DataType* OpBitwiseOR();
	DataType* OpBitwiseXOR();
	DataType* OpBitwiseMOD();
	DataType* OpBitwiseNOT();
	DataType* OpAdd();
	DataType* OpSubtract();
	DataType* OpMultiply();
	DataType* OpDivide();
 
	DataType* OpUnaryNeg();
	DataType* OpUnaryNot();
	DataType* OpUnaryLogicalNot();
};
