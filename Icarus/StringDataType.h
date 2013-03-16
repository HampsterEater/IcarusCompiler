// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"
#include "DataType.h"

class StringDataType : public ClassDataType
{
public:
		
	StringDataType();


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
		return 6;
	}	

	bool CastTo(DataType* dt, bool explicitCast=false);

	void PushLiteral(std::string value);
	void PushDefault();

	void NullifyVariable(VariableSymbol* variable);

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