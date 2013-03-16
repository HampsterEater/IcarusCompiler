// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

class VariableSymbol;
class ClassDataType;
class StringDataType;
enum Register;

// This is used to define the primitie version of a data type.
enum PrimitiveDataType
{
	PDT_BOOL = 0,
	PDT_BYTE,
	PDT_SHORT,
	PDT_INT,
	PDT_LONG,
	PDT_FLOAT,
	PDT_DOUBLE,
	PDT_STRING,
	PDT_VOID,
	PDT_CLASS
};
extern std::string PrimitiveDataTypeMnemonics[];

class DataType
{
public:

	std::string GetMnemonicForDataType(DataType* dt);

	virtual bool IsEqualTo(DataType* dt1)
	{
		return GetPrimitive() == dt1->GetPrimitive();
	}

	virtual bool RequiresCast(DataType* dt2);

	virtual PrimitiveDataType GetPrimitive()=0;
	virtual std::string GetAssemblyRefName()=0;
	virtual std::string GetAssemblyDefName()=0;
	virtual int GetStoredSize()=0;
	virtual int GetSize()=0;

	virtual bool CastTo(DataType* dt, bool explicitCast=false)
	{
		if (dt->GetPrimitive() == GetPrimitive())
			return true;

		switch (dt->GetPrimitive())
		{
			case PDT_BOOL: break;
			case PDT_BYTE: break;
			case PDT_SHORT: break;
			case PDT_INT: break;
			case PDT_LONG: break;
			case PDT_FLOAT: break;
			case PDT_DOUBLE: break;
			case PDT_STRING: break;
			case PDT_VOID: break;
			case PDT_CLASS: break;
		}
		return false;
	}
	virtual bool PromoteToParameter()
	{
		return false;
	}
	virtual int GetImplicityCastPriority()
	{
		return -1;
	}

	virtual void Pop(Register reg)=0;
	virtual void PopDestroy()=0;
	virtual void Push(Register reg)=0;
	virtual void PushLiteral(std::string value)=0;
	virtual void PushDefault()=0;
	virtual void PushReturn()=0;
	virtual void PushVariableValue(VariableSymbol* variable)=0;
	virtual void AssignToVariable(VariableSymbol* variable)=0;
	virtual void AssignToReturn()=0;
	virtual void NullifyVariable(VariableSymbol* variable)=0;

	virtual DataType* OpCompareLess()=0;
	virtual DataType* OpCompareGreater()=0;
	virtual DataType* OpCompareLessEqual()=0;
	virtual DataType* OpCompareGreaterEqual()=0;
	virtual DataType* OpCompareEqual()=0;
	virtual DataType* OpCompareNotEqual()=0;
	virtual DataType* OpBitwiseSHL()=0;
	virtual DataType* OpBitwiseSHR()=0;
	virtual DataType* OpBitwiseAND()=0;
	virtual DataType* OpBitwiseOR()=0;
	virtual DataType* OpBitwiseXOR()=0;
	virtual DataType* OpBitwiseMOD()=0;
	virtual DataType* OpBitwiseNOT()=0;
	virtual DataType* OpAdd()=0;
	virtual DataType* OpSubtract()=0;
	virtual DataType* OpMultiply()=0;
	virtual DataType* OpDivide()=0;
 
	virtual DataType* OpUnaryNeg()=0;
	virtual DataType* OpUnaryNot()=0;
	virtual DataType* OpUnaryLogicalNot()=0;
};






