// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
// WARNING: Keep these values syncronized with values in runtime.
// --------------------------------------------------------------------------------
#define ClassHeaderSize 36//sizeof(ClassObjectHeader)

#define META_TYPE_VARIABLE     1
#define META_TYPE_FUNCTION     2
#define META_TYPE_CLASS        3
#define META_TYPE_NAMESPACE    4

#define VFTABLE_CLASS_OFFSET 24

// --------------------------------------------------------------------------------
//	This struct is used to store details on the start and end jump targets
//	of a loop, which are then used in break/continue parsing to jump around 
//	the loop.
// --------------------------------------------------------------------------------
struct LoopTracker
{
	JumpTarget* ContinueTarget;
	JumpTarget* BreakTarget;

	LoopTracker(JumpTarget* conTarget, JumpTarget* breakTarget)
	{
		ContinueTarget = conTarget;
		BreakTarget = breakTarget;
	}
};

// --------------------------------------------------------------------------------
//	The parser is responsible for taking a list of tokens and 
//  generating an instruction tree.
// --------------------------------------------------------------------------------
class Parser
{
private:
	std::vector<Token> _tokenList;

	FunctionSymbol* _globalScope;
	Symbol* _currentScope;

	Token _currentToken;
	int _tokenIndex;

	int _currentPass;

	int _lastASMCommentEmitLine;

	int _externBlockDepth;

	int _registerAllocation[255];
	DataType* _registerAllocationDataType[255];

	Symbol* _overrideInstructionScope;

	std::vector<LoopTracker> _loopTrackerStack;

	int classDefinitionIndexCounter;

	AccessModifierLevel _currentAccessModifier;
	
public:
	Parser();

	bool Parse(std::vector<Token> tokenList);

	bool EndOfTokens();
	Token NextToken();
	Token LookAheadToken(int offset=1);
	Token ExpectToken(TokenID id);
	void CheckToken(TokenID id);

	Symbol* GetInstructionScope() { return (_overrideInstructionScope != NULL ? _overrideInstructionScope : _currentScope); }
	Token GetCurrentToken() { return _currentToken; } 

	VariableSymbol* ParseVariable(bool assignmentsAllowed=true);
	void ParseFunction();
	void ParseClass();
	void ParseBlock();
	void SkipBlock();
	void ParseReturn();
	void ParseIf();
	void ParseWhile();
	void ParseFor();
	void ParseForEach();
	void ParseContinue();
	void ParseBreak();
	void ParseSwitch();
	void ParseTry();
	void ParseThrow();
	void ParseAccessModifier();
	void ParseNamespace();
	void ParseAssignment(VariableSymbol* variable);

	int CalcFunctionParameters();
	int CalcFunctionCallParameters();

	DataType* ParseFunctionCall(FunctionSymbol* explicitSymbol=NULL, bool doNotUseVirtual=false, std::string name="", bool isMethod=false, bool doCall=true);

	DataType* ParseExpression();
	DataType* ParseExpressionLevel1();
	DataType* ParseExpressionLevel2();
	DataType* ParseExpressionLevel3();
	DataType* ParseExpressionLevel4();
	DataType* ParseExpressionLevel5();
	DataType* ParseExpressionLevel6();
	DataType* ParseExpressionLevel7();
	DataType* ParseExpressionLevel8();

	void WriteClassRegistrationCode(Symbol* symbol);

	void WriteFunctionPrefix(FunctionSymbol* functionSymbol);
	void WriteFunctionPostfix(FunctionSymbol* functionSymbol, bool finalReturn);
	Operand* GetVariableAccessOperand(VariableSymbol* variable, int offset=0, Register thisRegister=REG_EAX);

	void BuildClassHierarchy(Symbol* symbol);
	void BuildVirtualFunctions(Symbol* symbol);
	void BuildVariables(Symbol* symbol);

	//Register CastRegisterDataType(Register sourceRegister, DataType* sourceDataType, DataType* destDataType);

	void ParseStatement();
	DataType* ParseDataType(bool returnNullOnError=false);
	Symbol* ParseNamespaceScope();

	Register Register64HiBitsTwin(Register reg);
	Register Register64LoBitsTwin(Register reg);
		
	Register Register32BitTo8Bit(Register reg);
	Register Register32BitTo16Bit(Register reg);
	Register AllocateRegister(DataType* type);
	void DeallocateRegister(Register reg);
	void SetRegisterAllocation(Register reg, bool val);
	//Register AllocateRegister(DataType* type, int index);

	FunctionSymbol* GetGlobalScope() { return _globalScope; }

	StringSymbol* DefineStringLiteral(std::string value, bool isNullTerminated=false);

};