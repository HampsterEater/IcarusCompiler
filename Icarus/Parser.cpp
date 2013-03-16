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
//	Initializes the parser.
// --------------------------------------------------------------------------------
Parser::Parser()
{
	_overrideInstructionScope = NULL;
	classDefinitionIndexCounter = 0;
	for (int i = 0; i < 255; i++)
	{
		_registerAllocation[i] = false;
		_registerAllocationDataType[i] = NULL;
	}
}

// --------------------------------------------------------------------------------
//	Makes the parser generate an instruction tree out of a token list.
// --------------------------------------------------------------------------------
bool Parser::Parse(std::vector<Token> tokenList)
{	
	_tokenList = tokenList;
	_currentToken = Token(TOKEN_KEYWORD_FUNCTION, "", "", 0, 0, "");
	_tokenIndex = 0;
	_currentPass = 0;
	_lastASMCommentEmitLine = -1;
	_externBlockDepth = 0;

	// Create a function symbol to track the global scope.
	_globalScope = new FunctionSymbol("$_GLOBAL", NULL, _currentToken);
	DefineStringLiteral("$_GLOBAL", true);
	DefineStringLiteral("", true);

	// Parse the token list in several streams.
	// Pass 1:
	//	 - Create symbolic references for all constructs in the source file (classes, variables, etc).
	// Pass 1 and 1/2:
	//	 - Create a class heirarchy and patch up data types of variables/functions/etc.
	// Pass 2:
	//	 - Validate all references.
	//	 - General assembly output.
	for (_currentPass = 1; _currentPass <= 2; _currentPass++)
	{
		printf(" - - Pass %i ...\n", _currentPass);
		_currentToken = Token();
		_tokenIndex = 0;
		_currentScope = _globalScope;
		_lastASMCommentEmitLine = -1;
		_externBlockDepth = 0;
		_currentAccessModifier = AM_PUBLIC;
		
		// Add global prefix.
		WriteFunctionPrefix(_globalScope);

		// Add class registration code.
		WriteClassRegistrationCode(_globalScope);

		while (!EndOfTokens())
			ParseStatement();

		// Add global postfix.
		WriteFunctionPostfix(_globalScope, true);

		// If this is the first pass, we should build the class heirarchy
		// that will be used in the next pass.
		if (_currentPass == 1)
		{
			printf(" - - Building Class Hierarchy ...\n");
			BuildClassHierarchy(_globalScope);
			
			printf(" - - Building Virtual Function Tables ...\n");
			BuildVirtualFunctions(_globalScope);
			
			printf(" - - Calculating Variable Offsets ...\n");
			BuildVariables(_globalScope);
		}
	}
	
	return true;
}

// --------------------------------------------------------------------------------
//	Extends all classes from their base classes. 
//  Patchs variable/function data types (variables referencing classes, etc).
// --------------------------------------------------------------------------------
void Parser::BuildClassHierarchy(Symbol* symbol)
{
	// Patch everything up :3.
	symbol->BuildClassHierarchy();

	// Build heirarchys for all sub-symbols too.
	for (int i = 0; i < symbol->Children.size(); i++)
		BuildClassHierarchy(symbol->Children.at(i));	
}

// --------------------------------------------------------------------------------
//	Builds the virtual function table for all classes.
// --------------------------------------------------------------------------------
void Parser::BuildVirtualFunctions(Symbol* symbol)
{
	// Patch everything up :3.
	//ClassSymbol* clsSym = dynamic_cast<ClassSymbol*>(symbol);
	//if (clsSym != NULL)
	//	clsSym->BuildVirtualFunctions();

	// Build heirarchys for all sub-symbols too.
	ClassSymbol* clsSym = dynamic_cast<ClassSymbol*>(symbol->FindChild("object"));
	if (clsSym != NULL)
		clsSym->BuildVirtualFunctions();

	//for (int i = 0; i < symbol->Children.size(); i++)
	//{	
	//	ClassSymbol* clsSym = dynamic_cast<ClassSymbol*>(symbol->Children.at(i));
	//	if (clsSym != NULL)
	//		clsSym->BuildVirtualFunctions();
	//	BuildVirtualFunctions(symbol->Children.at(i));	
	//}
}

// --------------------------------------------------------------------------------
//	Builds the variables for this class!
// --------------------------------------------------------------------------------
void Parser::BuildVariables(Symbol* symbol)
{
	// Build heirarchys for all sub-symbols too.
	ClassSymbol* clsSym = dynamic_cast<ClassSymbol*>(symbol->FindChild("object"));
	if (clsSym != NULL)
		clsSym->BuildVariables();
}

// --------------------------------------------------------------------------------
//	Defines a string literal and stores it in the symbol table.
// --------------------------------------------------------------------------------
StringSymbol* Parser::DefineStringLiteral(std::string value, bool isNullTerminated)
{
	Symbol* str = _globalScope->FindString(value, isNullTerminated);
	if (str == NULL)
	{
		StringSymbol* strSym = new StringSymbol(value, _globalScope, _currentToken);
		strSym->IsNullTerminated = isNullTerminated;

		return strSym;
	}
	else
		return (StringSymbol*)str;
}

// --------------------------------------------------------------------------------
//	Writes out the class registration code.
// --------------------------------------------------------------------------------
void Parser::WriteClassRegistrationCode(Symbol* symbol)
{
	Instruction* instruction;

	if (_currentPass != 2)
		return;

	// Write out the global registration.
	if (symbol == _globalScope)
	{
		// Push the pointer to the class definition.
		instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
		instruction->AddLabelOperand(_globalScope, "_meta");

		// Call the registration code.
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__RegisterGlobal");

		// Restore the stack.
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(4);
	}

	// Write out the class registration.
//	if (symbol->GetSymbolType() == ST_CLASS)
//	{
//		ClassSymbol* classSymbol = (ClassSymbol*)symbol;
//
//		// Push the pointer to the class definition.
//		instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
//		instruction->AddLabelOperand(classSymbol);
//
//		// Call the registration code.
//		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
//		instruction->AddStringLiteralOperand("_icr__RegisterClass");
//
//		// Restore the stack.
//		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
//		instruction->AddRegisterOperand(REG_ESP);
//		instruction->AddIntLiteralOperand(4);
//	}

	// Write out the global registration.
//	if (symbol == _globalScope)
//	{
//		// Push the pointer to the class definition.
//		instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
//		instruction->AddLabelOperand(_globalScope, "_meta");
//
//		// Call the registration code.
//		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
//		instruction->AddStringLiteralOperand("_icr__RegisterGlobal");
//
//		// Restore the stack.
//		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
//		instruction->AddRegisterOperand(REG_ESP);
//		instruction->AddIntLiteralOperand(4);
//	}

	// Write out all the children symbols.
//	for (int i = 0; i < symbol->Children.size(); i++)
//		WriteClassRegistrationCode(symbol->Children.at(i));
}

// --------------------------------------------------------------------------------
//	Returns true if we are at the end of the token stream.
// --------------------------------------------------------------------------------
bool Parser::EndOfTokens()
{
	return (_tokenIndex >= (int)_tokenList.size());
}

// --------------------------------------------------------------------------------
//	Returns the next token in the stream.
// --------------------------------------------------------------------------------
Token Parser::NextToken()
{
	if (EndOfTokens()) 
		g_app->EmitError("An internal error occured. Parser attempted to read token past end of stream.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	_currentToken = _tokenList.at(_tokenIndex++);

	if (_currentPass == 2 && _currentToken.Line != _lastASMCommentEmitLine)
	{
		if (_currentToken.LineLiteral != "")
		{
			Instruction* instruction = new Instruction(_currentScope, OPCODE_COMMENT, _currentToken);
			instruction->AddStringLiteralOperand(_currentToken.LineLiteral);
		}

		_lastASMCommentEmitLine = _currentToken.Line;
	}

	// Define the source file this token is in as a string literal, 
	// we will need it when debugging / defining metadata.
//	if (_currentPass == 1)
//		DefineStringLiteral(_currentToken.Path);

	return _currentToken;
}

// --------------------------------------------------------------------------------
//	Returns the token at current_position + offset without advancing the stream.
// --------------------------------------------------------------------------------
Token Parser::LookAheadToken(int offset)
{
	offset--; // Remove one from the index seeing as tokenindex already 
			  // points to one token beyond our current one.
	if (_tokenIndex + offset >= (int)_tokenList.size())
		g_app->EmitError("An internal error occured. Parser expected look ahead token, but encountered end of file.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	return _tokenList.at(_tokenIndex + offset);
}

// --------------------------------------------------------------------------------
//	Reads in the next token and checks it has the given ID, if not it 
//  raises an error.
// --------------------------------------------------------------------------------
Token Parser::ExpectToken(TokenID id)
{
	NextToken();
	CheckToken(id);
	return _currentToken;
}

// --------------------------------------------------------------------------------
//	Checks the current token, if it dosen't have the ID given it raises an error.
// --------------------------------------------------------------------------------
void Parser::CheckToken(TokenID id)
{
	if (_currentToken.ID != id)
		g_app->EmitError("Encountered unexpected token \"" + _currentToken.Literal + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
}

// --------------------------------------------------------------------------------
//	Parses the highest level statement the language can be broken down into.
// --------------------------------------------------------------------------------
void Parser::ParseStatement()
{
	// End of stream?
	if (EndOfTokens())
		return;

	// Empty statement?
	if (LookAheadToken().ID == TOKEN_CHAR_SEMICOLON)
	{
		NextToken();
		return;
	}

	// Parse the next token in the list.
	switch (LookAheadToken().ID)
	{
		// Parse a block of statements
		case TOKEN_CHAR_OPEN_BRACE:
		{
			NextToken();
			ParseBlock();
			break;
		}

		// Parse an access modifier.
		case TOKEN_KEYWORD_PRIVATE:
		case TOKEN_KEYWORD_PUBLIC:
		case TOKEN_KEYWORD_PROTECTED:
		{
			NextToken();
			ParseAccessModifier();
			break;
		}
		
		// Parse a variable.
		case TOKEN_KEYWORD_STATIC:
		case TOKEN_KEYWORD_CONST:
		case TOKEN_KEYWORD_VAR:
		{
			NextToken();
			ParseVariable();
			break;
		}

		// Parse a function.
		case TOKEN_KEYWORD_METHOD:
		case TOKEN_KEYWORD_FUNCTION:
		case TOKEN_KEYWORD_GENERATOR:
		{
			NextToken();
			ParseFunction();
			break;
		}

		// Parse return.
		case TOKEN_KEYWORD_RETURN:
		{
			NextToken();
			ParseReturn();
			break;
		}

		// Parse if block.
		case TOKEN_KEYWORD_IF:
		{
			NextToken();
			ParseIf();
			break;
		}

		// Parse while block.
		case TOKEN_KEYWORD_WHILE:
		{
			NextToken();
			ParseWhile();
			break;
		}

		// Parse namespace block.
		case TOKEN_KEYWORD_NAMESPACE:
		{
			NextToken();
			ParseNamespace();
			break;
		}

		// Parse for block.
		case TOKEN_KEYWORD_FOR:
		{
			NextToken();
			ParseFor();
			break;
		}

		// Parse foreach block.
		case TOKEN_KEYWORD_FOREACH:
		{
			NextToken();
			ParseForEach();
			break;
		}

		// Parse switch block.
		case TOKEN_KEYWORD_SWITCH:
		{
			NextToken();
			ParseSwitch();
			break;
		}
		
		// Parse try/catch block.
		case TOKEN_KEYWORD_TRY:
		{
			NextToken();
			ParseTry();
			break;
		}
		
		// Parse throw statement.
		case TOKEN_KEYWORD_THROW:
		{
			NextToken();
			ParseThrow();
			break;
		}

		// Parse class block.
		case TOKEN_KEYWORD_CLASS:
		{
			NextToken();
			ParseClass();
			break;
		}

		// Parse break block.
		case TOKEN_KEYWORD_BREAK:
		{
			NextToken();
			ParseBreak();
			break;
		}

		// Parse continue block.
		case TOKEN_KEYWORD_CONTINUE:
		{
			NextToken();
			ParseContinue();
			break;
		}

		// Parse extern block.
		case TOKEN_KEYWORD_EXTERN:
		{
			NextToken();
			_externBlockDepth++;
			ParseBlock();
			_externBlockDepth--;
			break;
		}

		// Parses an identifier. 
		case TOKEN_CHAR_OPEN_PARENTHESIS:
		case TOKEN_LITERAL_IDENTIFIER:
		{
			DataType* type = ParseExpression();
			if (_currentPass == 2 && type->GetPrimitive() != PDT_VOID)
			{
				//Register register1 = AllocateRegister(type, 0);

				// Pop off to random register of the right size.
				type->PopDestroy();
				//Instruction* instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
				//instruction->AddRegisterOperand(register1);
			}
			
			if (LookAheadToken().ID == TOKEN_CHAR_SEMICOLON)
				ExpectToken(TOKEN_CHAR_SEMICOLON);

			break;
		}

		// WTF is this token doing here, naught programmer?
		default:
		{
			DataType* dt = ParseExpression();

			//if (_currentPass == 2 && dt->GetPrimitive() == PDT_VOID)
			//	g_app->EmitError("Encountered unexpected token \"" + LookAheadToken().Literal + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
			
			if (_currentPass == 2 && dt->GetPrimitive() != PDT_VOID)
				dt->PopDestroy();

			break;
		}
	}

	// Lets stick a garbage collection call after this statement?
	
	if (_currentPass == 2)
	{
		bool isLastInstructionGC = false;
		
		if (_currentScope->Instructions.size() > 0)
		{
			Instruction* lastInstruction = NULL;
			for (int i = _currentScope->Instructions.size() - 1; i >= 0; i--)
			{
				if (_currentScope->Instructions.at(i)->OpCode != OPCODE_COMMENT &&
					_currentScope->Instructions.at(i)->OpCode != OPCODE_JUMP_TARGET)
				{
					lastInstruction = _currentScope->Instructions.at(i);
					break;
				}
			}

			if (lastInstruction != NULL)
			{
				if (lastInstruction->OpCode == OPCODE_CALL)
				{
					std::string opAsm = lastInstruction->Operands[0]->CreateAssembly();
					if (opAsm == "_icr__ExecuteGC")
						isLastInstructionGC = true;
				}
			}
		}

		// Don't add GC commands to new/delete methods.
		FunctionSymbol* func = dynamic_cast<FunctionSymbol*>(_currentScope);
		if (func != NULL && (func->Identifier == "new" || func->Identifier == "delete" || func->Identifier == "__construct" || func->Identifier == "__dispose"))
			isLastInstructionGC = true;

		if (isLastInstructionGC == false)
		{
			// Invoke garbage collection function.
			Instruction* instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
			instruction->AddStringLiteralOperand("_icr__ExecuteGC");
		}
	}
	
}

// --------------------------------------------------------------------------------
//	Parses a variable declaration.
//
//		var type name = 0;
//		static type name = 0;
//		const type name = 0;
// --------------------------------------------------------------------------------
VariableSymbol* Parser::ParseVariable(bool assignmentsAllowed)
{
//	Instruction* instruction;
	std::string identifier;
	
	DataType* dataType = new VoidDataType();
	VariableSymbol* variableSymbol = NULL;
	
	// Store the starting token so we know 
	// what type of variable it is.
	Token startToken = _currentToken;
	
	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION &&
		_currentScope->GetSymbolType() != ST_CLASS &&
		_currentScope->GetSymbolType() != ST_NAMESPACE)
		g_app->EmitError("Variables can only be declared within a function, class, namespace or global scope.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Parse the data type.
	dataType = ParseDataType();

	while (true)
	{
		// Read in the identifier.
		identifier = ExpectToken(TOKEN_LITERAL_IDENTIFIER).Literal;
	
		// If we are in pass one, then create the symbol.
		if (_currentPass == 1)
		{
			// Check no other symbols exist with the same name.
			if (_currentScope->FindChild(identifier, false, false) != NULL)
				g_app->EmitError("Redefinition of identifier \"" + identifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

			// Create the variable symbol.
			variableSymbol = new VariableSymbol(identifier, _currentScope, _currentToken);
			switch (startToken.ID)
			{
				case TOKEN_KEYWORD_STATIC:
					{
						variableSymbol->VariableType = VT_GLOBAL;
						break;
					}
				case TOKEN_KEYWORD_VAR:
					{
						if (_currentScope->GetSymbolType() == ST_CLASS)
						{
							ClassSymbol* classSymbol = (ClassSymbol*)_currentScope;

							// Done in ClassSymbol->BuildClassHeirarchy now
							variableSymbol->VariableType = VT_FIELD;
							variableSymbol->ParentClass = classSymbol;

						
						//	variableSymbol->VariableType = VT_FIELD;
						//	variableSymbol->ClassOffset = classSymbol->MemoryDataSize;

						//	classSymbol->MemoryDataSize += dataType->GetSize();
							//functionSymbol->LocalCount++;
							//functionSymbol->LocalSize += dataType->GetSize();					
						}
						else
						{
							variableSymbol->VariableType = _currentScope == _globalScope ? VT_GLOBAL : VT_LOCAL;
							if (_currentScope != _globalScope)
							{
								FunctionSymbol* functionSymbol = (FunctionSymbol*)_currentScope;
								functionSymbol->LocalCount++;
								functionSymbol->LocalSize += dataType->GetSize();
							
								//variableSymbol->StackOffset = (functionSymbol->ParameterSize + functionSymbol->LocalSize);
							}
						}
						break;
					}
				case TOKEN_KEYWORD_CONST:
					{
						variableSymbol->VariableType = VT_CONST;
						break;
					}
				default:
					{
						g_app->EmitError("Invalid starting token for variable \"" + startToken.Literal + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
						break;
					}
			}
			variableSymbol->DataType = dataType;		
			variableSymbol->AccessModifier = _currentAccessModifier;

			// Define the name and token paths as string symbols for the var definitions.
			DefineStringLiteral(variableSymbol->Identifier, true);
			DefineStringLiteral(variableSymbol->DefinitionToken.Path, true);
		}

		// If we are in pass two, then update the symbol. 
		else if (_currentPass == 2)
		{
			variableSymbol = (VariableSymbol*)_currentScope->FindChild(identifier);
			variableSymbol->DataType = dataType;
		}

		// Parse the assignment if we have one.
		if (LookAheadToken().ID == TOKEN_OP_ASSIGN)
		{	
	//		if (variableSymbol->VariableType == VT_FIELD)
	//			g_app->EmitError("TODO: Default assignments can not be performed on field variables.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
			if (assignmentsAllowed == false)
				g_app->EmitError("Assignments are not allowed in this variable declaration.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

			// Read in assignment op.
			ExpectToken(TOKEN_OP_ASSIGN);

			// Parse the expression that follows.
			DataType* returnType = ParseExpression();

			if (_currentPass == 2)
			{				
				// Cast to variables data type.
				returnType->CastTo(variableSymbol->DataType);
				returnType = variableSymbol->DataType;

				// Assign value to variable.
				returnType->AssignToVariable(variableSymbol);

				/*
				Register register1 = AllocateRegister(returnType, 0);

				// Pop value off of stack.
				instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
				instruction->AddRegisterOperand(register1);
			
				// Cast to correct type.
				register1 = CastRegisterDataType(register1, returnType, variableSymbol->DataType);
		
				// Set value of variable to return value.
				instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
				instruction->AddOperand(GetVariableAccessOperand(variableSymbol));
				instruction->AddRegisterOperand(register1);
				*/
			}
		}
		else
		{
			if (variableSymbol->VariableType == VT_CONST || variableSymbol->IsConst == true)
				g_app->EmitError("Expecting assignment for constant variable \"" + identifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
		}

		// Read comma?
		if (LookAheadToken().ID == TOKEN_CHAR_COMMA)
			ExpectToken(TOKEN_CHAR_COMMA);
		else
			break;
	}

	// Read in the ending semi-colon.
	if (assignmentsAllowed == true)
		ExpectToken(TOKEN_CHAR_SEMICOLON);

	return variableSymbol;
}

// --------------------------------------------------------------------------------
//	Parses an access variable modifier.
//
//		public:
//		private:
//		public:
// --------------------------------------------------------------------------------
void Parser::ParseAccessModifier()
{
	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_CLASS)
		g_app->EmitError("Access modifiers can only be declared within a class declaration.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Work out what kind of access modifier it is.
	switch (_currentToken.ID)
	{
		case TOKEN_KEYWORD_PUBLIC:
			_currentAccessModifier = AM_PUBLIC;
			break;

		case TOKEN_KEYWORD_PRIVATE:
			_currentAccessModifier = AM_PRIVATE;
			break;

		case TOKEN_KEYWORD_PROTECTED:
			_currentAccessModifier = AM_PROTECTED;
			break;
	}

	// Parse the optional colon at the end.
	if (LookAheadToken().ID == TOKEN_CHAR_COLON)
		ExpectToken(TOKEN_CHAR_COLON);
}

// --------------------------------------------------------------------------------
//	Parses a return from function call.
//
//		return;
//		return (expr);
// --------------------------------------------------------------------------------
void Parser::ParseReturn()
{
//	Instruction* instruction;

	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("The return keyword can only be used within a function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Is there a return expression?
	DataType* returnDataType = new VoidDataType();
	if (LookAheadToken().ID != TOKEN_CHAR_SEMICOLON)
	{
		returnDataType = ParseExpression();

		if (_currentPass == 2)
		{
			FunctionSymbol* functionSymbol = ((FunctionSymbol*)_currentScope);
			functionSymbol->ReturnExists = true;

			// Cast to functions return data type.
			returnDataType->CastTo(functionSymbol->ReturnDataType);
			returnDataType =  functionSymbol->ReturnDataType;

			// Assign value to return register.
			returnDataType->AssignToReturn();

			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(REG_EAX);

			WriteFunctionPostfix((FunctionSymbol*)_currentScope, false);
		}
	}
	else
	{
		if (_currentPass == 2)
		{
			FunctionSymbol* functionSymbol = ((FunctionSymbol*)_currentScope);
			functionSymbol->ReturnExists = true;

			WriteFunctionPostfix((FunctionSymbol*)_currentScope, true);
		}
	}

	// Check if we are expecting a return type.
	if (_currentPass == 2 && returnDataType->GetPrimitive() != ((FunctionSymbol*)_currentScope)->ReturnDataType->GetPrimitive())
		g_app->EmitError("Function expects return value, but none was given.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Read in the ending semi-colon.
	ExpectToken(TOKEN_CHAR_SEMICOLON);
}

// --------------------------------------------------------------------------------
//	Parses a switch statement.
//
//		switch (expr)
//		{
//			case expr:
//			{
//				Break;
//			}
//			default:
//			{
//
//			}
//		}
// --------------------------------------------------------------------------------
void Parser::ParseSwitch()
{
	Instruction* instruction = NULL;
	JumpTarget* endJumpTarget = NULL;
	JumpTarget* defaultJumpTarget = NULL;
	JumpTarget* caseJumpTarget = NULL;
	bool defaultBlockExists = false;
	std::vector<JumpTarget*> _caseStatementJumpTargets;

	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("The switch keyword can only be used within a function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
	// Create a jump targets.
	if (_currentPass == 2)
	{
		endJumpTarget = new JumpTarget(_currentToken);
		defaultJumpTarget = new JumpTarget(_currentToken);
		defaultBlockExists = false;
	}

	// Read in the switch expression.
	DataType* switchDataType = ParseExpression();

	// Read in the main case block.
	ExpectToken(TOKEN_CHAR_OPEN_BRACE);

	// Create a loop tracker.
	_loopTrackerStack.push_back(LoopTracker(NULL, endJumpTarget));

	// This run we are going to build the jump table.
	int startPassTokenIndex = _tokenIndex;
	int caseBlockIndex = 0;
	for (int pass = 0; pass < 2; pass++)
	{
		// Reset to start of switch block.
		caseBlockIndex = 0;
		_tokenIndex = startPassTokenIndex - 1;
		NextToken();

		// Parse through entire block.
		while (true)
		{
			// End of the token stream?
			if (EndOfTokens())
				break;

			// End of block.
			if (LookAheadToken().ID == TOKEN_CHAR_CLOSE_BRACE)
				break;

			// Read in the next token so we know what to do.
			switch (LookAheadToken().ID)
			{
				case TOKEN_KEYWORD_CASE:
					{
						NextToken();
						
						// If in first pass, then build jump table.
						if (pass == 0)
						{
							// Parse the case expression.	
							DataType* exprType = ParseExpression();
			
							// Parse a colon if there is one, some people just like C's way of doing things :P
							if (LookAheadToken().ID == TOKEN_CHAR_COLON)
								ExpectToken(TOKEN_CHAR_COLON);

							// Build the jump code.
							if (_currentPass == 2)
							{
								caseJumpTarget = new JumpTarget(_currentToken);
								
								// Not the switch block type? Cast!
								exprType->CastTo(switchDataType);
								exprType = switchDataType;

								// Allocate registers for comparison.
								Register switchValueReg = AllocateRegister(switchDataType);
								Register caseValueReg = AllocateRegister(switchDataType);

								// Pop values out and into registers.
								exprType->Pop(caseValueReg);
								switchDataType->Pop(switchValueReg);

								// Push values back in.
								switchDataType->Push(switchValueReg); // Push switch in twice so we still have the value when its poped out by comparison operator.
								switchDataType->Push(switchValueReg);
								exprType->Push(caseValueReg);
								
								// Deallocate registers.
								DeallocateRegister(caseValueReg);
								DeallocateRegister(switchValueReg);

								// Compare values with each other.
								exprType->OpCompareEqual();
								
								// Allocate registers.
								DataType* boolDT = new BoolDataType();
								Register boolCompareReg = AllocateRegister(boolDT);

								// Pop out comparison result.
								boolDT->Pop(boolCompareReg);

								// Compare value to 0.
								instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
								instruction->AddRegisterOperand(boolCompareReg);
								instruction->AddIntLiteralOperand(0);

								// Jump to case code if equal.
								instruction = new Instruction(_currentScope, OPCODE_JNE, _currentToken);
								instruction->AddLabelOperand(caseJumpTarget);

								// Deallocate registers.
								DeallocateRegister(boolCompareReg);

								// Push jump label.
								_caseStatementJumpTargets.push_back(caseJumpTarget);
							}

							// Skip the case block.
							SkipBlock();
						}

						// If in second pass, then build case block.
						else if (pass == 1)
						{
							// Parse the case expression.	
							int oldPass = _currentPass;
							_currentPass = -99; // Stop instructions being generated.
							DataType* exprType = ParseExpression();
							_currentPass = oldPass;
	
							// Parse a colon if there is one, some people just like C's way of doing things :P
							if (LookAheadToken().ID == TOKEN_CHAR_COLON)
								ExpectToken(TOKEN_CHAR_COLON);
							
							// Bind the jump label.
							if (_currentPass == 2)
								_caseStatementJumpTargets.at(caseBlockIndex)->Bind(_currentScope);

							// Parse the case block.
							ParseBlock();
							
							// Jump to the end label.
							if (_currentPass == 2)
							{
								instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
								instruction->AddLabelOperand(endJumpTarget);
							}
						}

						caseBlockIndex++;

						break;
					}
				case TOKEN_KEYWORD_DEFAULT:
					{
						NextToken();

						// Parse a colon if there is one, some people just like C's way of doing things :P
						if (LookAheadToken().ID == TOKEN_CHAR_COLON)
							ExpectToken(TOKEN_CHAR_COLON);

						// Just skip the default block code here.
						if (pass == 0)
						{	
							if (_currentPass == 2)
								defaultJumpTarget = new JumpTarget(_currentToken);

							// Skip the default block.
							SkipBlock();
						}

						// Parse the code for the default block.
						else if (pass == 1)
						{
							// Bind the jump target.
							if (_currentPass == 2)
								defaultJumpTarget->Bind(_currentScope);

							// Parse the default block.
							ParseBlock();

							// Jump to the end label.
							if (_currentPass == 2)
							{
								instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
								instruction->AddLabelOperand(endJumpTarget);
							}
						}
						
						defaultBlockExists = true;

						break;
					}
				default:
					{
						g_app->EmitError("Encountered unexpected token \"" + LookAheadToken().Literal + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
						break;
					}
			}
		}

		// Add the code to jump to the default block when building the jump table.
		if (pass == 0 && _currentPass == 2)
		{
			instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
			instruction->AddLabelOperand(defaultBlockExists == true ? defaultJumpTarget : endJumpTarget);	
		}
	}
	
	// Pop a loop tracker.
	_loopTrackerStack.pop_back();

	// Bind the end target.
	if (_currentPass == 2)
		endJumpTarget->Bind(_currentScope);
	
	// Pop out value we were testing! 
	if (_currentPass == 2)
	{
		switchDataType->PopDestroy();
	//	switchDataType->PopDestroy();
	}

	// Read in closing brace.
	ExpectToken(TOKEN_CHAR_CLOSE_BRACE);
}

// --------------------------------------------------------------------------------
//	Parses a throw exception statement :3
//
//		return (expr);
// --------------------------------------------------------------------------------
void Parser::ParseThrow()
{
//	Instruction* instruction;

	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("The throw keyword can only be used within a function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Is there a return expression?
	DataType* returnDataType = ParseExpression();

	if (_currentPass == 2)
	{
		// Cast to functions return data type.
		ClassDataType* dst = new ClassDataType((ClassSymbol*)_currentScope->FindChild("object", false, true));
		returnDataType->CastTo(dst);
		returnDataType = dst;

		// Check return value is class type.
		//if (returnDataType->GetPrimitive() != PDT_CLASS)
		//	g_app->EmitError("Throw values must be of class type.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		// Call throw function.
		Instruction* instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__ExceptionThrow");
	}

	// Read in the ending semi-colon.
	ExpectToken(TOKEN_CHAR_SEMICOLON);
}

// --------------------------------------------------------------------------------
//	Parses a try block.
//
//		try
//		{	
//		}
//		catch (object e)
//		{
//		}
//		catch
//		{
//		}
//
//
//		TODO: Implement finally/else
//
// --------------------------------------------------------------------------------
void Parser::ParseTry()
{
	Instruction* instruction = NULL;
	JumpTarget* exceptionHandlerJumpTarget = NULL;
	JumpTarget* endOfTryJumpTarget = NULL;
	JumpTarget* endOfExceptionHandlerJumpTarget = NULL;
	JumpTarget* finallyJumpTarget = NULL;
	JumpTarget* completeEndOfTryJumpTarget = NULL;
	JumpTarget* elseJumpTarget = NULL;
	JumpTarget* noExceptionJumpTarget = NULL;


	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("Try/Catch blocks can only be used within a function or the global scope.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Invoke the execution stack enter function.
	if (_currentPass == 2)
	{
		// Generate jump targets.
		exceptionHandlerJumpTarget = new JumpTarget(_currentToken);
		endOfTryJumpTarget = new JumpTarget(_currentToken);
		endOfExceptionHandlerJumpTarget = new JumpTarget(_currentToken);
		completeEndOfTryJumpTarget = new JumpTarget(_currentToken);
		noExceptionJumpTarget = new JumpTarget(_currentToken);

		// Invoke the exception stack's enter function
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__ExceptionStackEnter");
				
		// Push the state into the stack.
		instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
		instruction->AddRegisterOperand(REG_EAX);

		// Invoke the function to get the current cpu state.
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__ExceptionGetCPUState");

		// Notice: This is where we will return to when an exception is thrown.

		// Cleanup Stack
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(4);

		// Check exception is not equal to null.
		instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
		instruction->AddRegisterOperand(REG_EAX);
		instruction->AddIntLiteralOperand(0);

		// If we have an exception in eax, then jump da fuck to the exception handler.
		instruction = new Instruction(_currentScope, OPCODE_JNE, _currentToken);
		instruction->AddLabelOperand(exceptionHandlerJumpTarget);
	}

	// Prase the try block.
	ParseBlock();

	// Write out the exception handler. 
	if (_currentPass == 2)
	{
		// Invoke the function to leave the current try block.
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__ExceptionStackLeave");

		// Jump to end of the try code if we haven't had an exception.
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(noExceptionJumpTarget);

		// Mark the label we will jump to on an exception.
		exceptionHandlerJumpTarget->Bind(_currentScope);

		// Push dat exception into stack.
		instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
		instruction->AddRegisterOperand(REG_EAX);
	}

	// Write out each catch block.
	bool foundCatchAllBlock = false;
	while (!EndOfTokens())
	{
		// Check what follows next is a valid block.
		Token lat = LookAheadToken();
		if (lat.ID != TOKEN_KEYWORD_CATCH &&
			lat.ID != TOKEN_KEYWORD_FINALLY &&
			lat.ID != TOKEN_KEYWORD_ELSE)
			break;

		// Check next token.
		switch (NextToken().ID)
		{		
			// Catch block.
			//
			// catch {}
			// catch (object name) {}
			case TOKEN_KEYWORD_CATCH:
				{
					// Parse the variable we will be putting the exception into.
					VariableSymbol* variable = NULL;
					JumpTarget* blockEndJumpTarget = NULL;
					
					lat = LookAheadToken();
					if (lat.ID == TOKEN_CHAR_OPEN_PARENTHESIS)
					{
						ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
						ExpectToken(TOKEN_KEYWORD_VAR);
						variable = ParseVariable(false);
						ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
					}

					// Check to see if exception is valid for this block.
					if (_currentPass == 2)
					{
						blockEndJumpTarget = new JumpTarget(_currentToken);

						// Catch-specific type.
						if (variable != NULL)
						{
							// Check variable is a class type!
							if (variable->DataType->GetPrimitive() != PDT_CLASS)
								g_app->EmitError("Catch block variables must be of class type.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

							ClassDataType* classDT = (ClassDataType*)(variable->DataType);

							// Check we have not found catch-all block yet.
							if (foundCatchAllBlock == true)
								g_app->EmitError("Catch blocks cannot follow a catch-all block!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

							// Pop exception into register.
							//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
							//instruction->AddRegisterOperand(REG_EAX);

							// Push exception back into stack (needs to stay there!).
							//instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
							//instruction->AddRegisterOperand(REG_EAX);

							// Push in the class index of whatever we want to cast to.
							instruction = new Instruction(_currentScope, OPCODE_PUSHD, _currentToken);
							instruction->AddIntLiteralOperand(classDT->Class->ClassDefinitionIndex);
					
							// Try and cast exception to type.
							instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
							instruction->AddStringLiteralOperand("_icr__ExplicitCast");
						
							// Clean up stack.
							instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
							instruction->AddRegisterOperand(REG_ESP);
							//instruction->AddIntLiteralOperand(8);
							instruction->AddIntLiteralOperand(4); // Only pop off class-index, we want to keep other stuff in stack :3.
					
							// See if downcast failed or not.
							instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
							instruction->AddRegisterOperand(REG_EAX);
							instruction->AddIntLiteralOperand(0);

							// If this block dosen't handle this type, jump out.
							instruction = new Instruction(_currentScope, OPCODE_JE, _currentToken);
							instruction->AddLabelOperand(blockEndJumpTarget);

							// Set variables value to exception.
							instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
							instruction->AddOperand(GetVariableAccessOperand(variable));
							instruction->AddRegisterOperand(REG_EAX);
						}

						// Catch-all.
						else
						{
							// Nothing to do: Always run block :3.

							foundCatchAllBlock = true;
						}
					}

					// Parse the actual exception block.
					ParseBlock();

					// End of block code.
					if (_currentPass == 2)
					{				
						// Jump to outside of try block as we have finished handling!
						instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
						instruction->AddLabelOperand(endOfExceptionHandlerJumpTarget);

						// Bind the end of this try block.
						blockEndJumpTarget->Bind(_currentScope);
					}

					break;
				}

			// Finally block.
			//
			// finally {}
			case TOKEN_KEYWORD_FINALLY:
				{
					if (_currentPass == 2)
					{
						finallyJumpTarget = new JumpTarget(_currentToken);
						finallyJumpTarget->Bind(_currentScope);
					}

					ParseBlock();
					
					if (_currentPass == 2)
					{
						instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
						instruction->AddLabelOperand(completeEndOfTryJumpTarget);
					}					
					break;
				}
				
			// Else block.
			//
			// finally {}
			case TOKEN_KEYWORD_ELSE:
				{
					if (_currentPass == 2)
					{
						elseJumpTarget = new JumpTarget(_currentToken);
						elseJumpTarget->Bind(_currentScope);
					}

					ParseBlock();
					
					if (_currentPass == 2)
					{
						instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
						instruction->AddLabelOperand(endOfTryJumpTarget);
					}					
					break;
				}
		}
	}

	// Write out the end-of-try code.
	if (_currentPass == 2)
	{
		// If we got here without executing anything, then throw another exception for a higher try statement to catch!
		// Assumes exception is still in stack - Which it should be if everything went correctly.
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__ExceptionThrow");

		// Bind the end of the try block.
		endOfExceptionHandlerJumpTarget->Bind(_currentScope);

		// Pop exception out of the stack.
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(4);
		
		// Jump to end of try block :) (Jump over else!)
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(endOfTryJumpTarget);

		// No-Exception jump target!
		noExceptionJumpTarget->Bind(_currentScope);

		if (elseJumpTarget != NULL)
		{
			instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
			instruction->AddLabelOperand(elseJumpTarget);
		}

		// Bind the end of the try block.
		endOfTryJumpTarget->Bind(_currentScope);

		// Jump to finally block.
		if (finallyJumpTarget != NULL)
		{
			instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
			instruction->AddLabelOperand(finallyJumpTarget);
		}

		// Bind the end of the try block.
		completeEndOfTryJumpTarget->Bind(_currentScope);
	}
}

// --------------------------------------------------------------------------------
//	Parses a class statement.
//
//		class <name> : <name>
//		{
//			<fields/methods>
//		}
// --------------------------------------------------------------------------------
void Parser::ParseClass()
{
	ClassSymbol* classSymbol = NULL;
	std::string identifier;
	ClassSymbol* extensionClass = NULL;
	std::string extendsIdentifier;
	Instruction* instruction = NULL;
	VariableSymbol* parameterSymbol = NULL;

	// Check we are in a valid scope.
	if (_currentScope != _globalScope && _currentScope->GetSymbolType() != ST_NAMESPACE)
		g_app->EmitError("Classes can only be declared within the global or namespace scope.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Read in the identifier.
	identifier = ExpectToken(TOKEN_LITERAL_IDENTIFIER).Literal;

	// Read in extends identifier.
	Symbol* extendsNamespaceScope = _globalScope;
	if (LookAheadToken().ID == TOKEN_CHAR_COLON)
	{
		//Symbol* symbol = ParseSymbol();
		NextToken();
		extendsIdentifier = ExpectToken(TOKEN_LITERAL_IDENTIFIER).Literal;
		extendsNamespaceScope = ParseNamespaceScope();
		extendsIdentifier = _currentToken.Literal;
	}
	else if (identifier != "object")
		extendsIdentifier = "object";

	// If we are in pass one, then create the symbol.
	if (_currentPass == 1)
	{
		// Check no other symbols exist with the same name.
		if (_currentScope->FindChild(identifier, false, false) != NULL)
			g_app->EmitError("Redefinition of identifier \"" + identifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		// Create the class symbol.
		classSymbol = new ClassSymbol(identifier, _currentScope, _currentToken);		
		classSymbol->ClassDefinitionIndex = classDefinitionIndexCounter++;
		classSymbol->ExtendsClassIdentifier = extendsIdentifier;

		if (identifier != "string" && identifier != "object")
		{
			// Define the internal dispose method.
			classSymbol->InternalConstructMethod = new FunctionSymbol("__construct", classSymbol, _currentToken);
			classSymbol->InternalConstructMethod->ReturnDataType = new VoidDataType();
			classSymbol->InternalConstructMethod->ParameterCount = 0;
			classSymbol->InternalConstructMethod->ParameterSize = 0;
			classSymbol->InternalConstructMethod->LocalSize = 0;
			classSymbol->InternalConstructMethod->LocalCount = 0;
			classSymbol->InternalConstructMethod->IsExtern = false;
			classSymbol->InternalConstructMethod->IsMethod = false; // Technically is a method, but say no to stop it being added to vftable and all that jazz.
			classSymbol->InternalConstructMethod->AccessModifier = AM_PRIVATE;
			DefineStringLiteral(classSymbol->InternalConstructMethod->Identifier, true);

			classSymbol->InternalDisposeMethod = new FunctionSymbol("__dispose", classSymbol, _currentToken);
			classSymbol->InternalDisposeMethod->ReturnDataType = new VoidDataType();
			classSymbol->InternalDisposeMethod->ParameterCount = 0;
			classSymbol->InternalDisposeMethod->ParameterSize = 0;
			classSymbol->InternalDisposeMethod->LocalSize = 0;
			classSymbol->InternalDisposeMethod->LocalCount = 0;
			classSymbol->InternalDisposeMethod->IsExtern = false;
			classSymbol->InternalDisposeMethod->IsMethod = false; // Technically is a method, but say no to stop it being added to vftable and all that jazz.
			classSymbol->InternalDisposeMethod->AccessModifier = AM_PRIVATE;
			DefineStringLiteral(classSymbol->InternalDisposeMethod->Identifier, true);

			// Define "this" for construct.
			parameterSymbol = new VariableSymbol("this", classSymbol->InternalConstructMethod, _currentToken);
			parameterSymbol->DataType = new ClassDataType(classSymbol);
			parameterSymbol->VariableType = VT_PARAMETER;
			parameterSymbol->IsConst = true;
			parameterSymbol->StackOffset = 8;
			parameterSymbol->ParamStackOffset = 8;
			classSymbol->InternalConstructMethod->ParameterCount++;
			classSymbol->InternalConstructMethod->ParameterSize += parameterSymbol->DataType->GetSize();
		
			// Define "this" for dispose.
			parameterSymbol = new VariableSymbol("this", classSymbol->InternalDisposeMethod, _currentToken);
			parameterSymbol->DataType = new ClassDataType(classSymbol);
			parameterSymbol->VariableType = VT_PARAMETER;
			parameterSymbol->IsConst = true;
			parameterSymbol->StackOffset = 8;
			parameterSymbol->ParamStackOffset = 8;
			classSymbol->InternalDisposeMethod->ParameterCount++;
			classSymbol->InternalDisposeMethod->ParameterSize += parameterSymbol->DataType->GetSize();
		}

		// Define the name and token paths as string symbols for the class definitions.
		DefineStringLiteral("this", true);
		DefineStringLiteral(classSymbol->Identifier, true);
		DefineStringLiteral(classSymbol->DefinitionToken.Path, true);
	}

	// If we are in pass two, then update the symbol. 
	else if (_currentPass == 2)
	{
		classSymbol = (ClassSymbol*)_currentScope->FindChild(identifier);

		extensionClass = (ClassSymbol*)extendsNamespaceScope->FindChild(extendsIdentifier);
		if (extensionClass == NULL && identifier != "object")
			g_app->EmitError("Undefined base class \"" + extendsIdentifier + "\" for class \"" + identifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

//		classSymbol->ExtendClass(extensionClass);
	}

	// Set the default access modifier.
	_currentAccessModifier = AM_PUBLIC;

	// Parse the class block.
	Symbol* previousScope = _currentScope;
	_currentScope = classSymbol;
	ParseBlock();
	_currentScope = previousScope;

	if (_currentPass == 2 && identifier != "string" && identifier != "object")
	{
		classSymbol = (ClassSymbol*)_currentScope->FindChild(identifier);
		
		// --------------------------------------------------------------------------------
		// Construct our dispose method.
		// --------------------------------------------------------------------------------
		FunctionSymbol* funcSymbol = classSymbol->InternalDisposeMethod;
		VariableSymbol* thisVar = NULL;
		_currentScope = classSymbol->InternalDisposeMethod;
		WriteFunctionPrefix(funcSymbol);
		
		instruction = new Instruction(funcSymbol, OPCODE_COMMENT, _currentToken);
		instruction->AddStringLiteralOperand("\n\t; Call Super-Destructor");
		
		thisVar = dynamic_cast<VariableSymbol*>(funcSymbol->Children.at(0));

		// Call super-class dispose method.
		if (classSymbol->SuperClass != NULL && classSymbol->SuperClass->InternalDisposeMethod != NULL)
		{
			// Push in the value of this.
			thisVar->DataType->PushVariableValue(thisVar);

			// Invoke function.
			instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
			instruction->AddLabelOperand(classSymbol->SuperClass->InternalDisposeMethod);

			// Deallocate space.
			instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
			instruction->AddRegisterOperand(REG_ESP);
			instruction->AddIntLiteralOperand(funcSymbol->ParameterSize);
		}
		
		instruction = new Instruction(funcSymbol, OPCODE_COMMENT, _currentToken);
		instruction->AddStringLiteralOperand("\n\t; Nullify Variables");

		// Push in the value of this.
		thisVar->DataType->PushVariableValue(thisVar);
		
		// Nullify all variables!
		for (int i = 0; i < classSymbol->Children.size(); i++)
		{
			Symbol* sym = classSymbol->Children.at(i);
			if (sym->GetSymbolType() == ST_VARIABLE && sym->Scope == classSymbol)
			{		
				instruction = new Instruction(funcSymbol, OPCODE_COMMENT, _currentToken);
				instruction->AddStringLiteralOperand("\n\t; Null: " + sym->Identifier);
				VariableSymbol* var = ((VariableSymbol*)sym);
				var->DataType->NullifyVariable(var);
			}
		}

		// Pop this back out.
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(4);
		
		WriteFunctionPostfix(funcSymbol, true);
		_currentScope = previousScope;	

		// --------------------------------------------------------------------------------
		// Construct our construct method.
		// --------------------------------------------------------------------------------
		funcSymbol = classSymbol->InternalConstructMethod;
		_currentScope = classSymbol->InternalConstructMethod;
		WriteFunctionPrefix(funcSymbol);
		
		instruction = new Instruction(funcSymbol, OPCODE_COMMENT, _currentToken);
		instruction->AddStringLiteralOperand("\n\t; Call Super-Constructor");
		
		thisVar = dynamic_cast<VariableSymbol*>(funcSymbol->Children.at(0));
		
		// Call super-construct dispose method.
		if (classSymbol->SuperClass != NULL && classSymbol->SuperClass->InternalConstructMethod != NULL)
		{
			// Push in the value of this.
			thisVar->DataType->PushVariableValue(thisVar);

			// Invoke function.
			instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
			instruction->AddLabelOperand(classSymbol->SuperClass->InternalConstructMethod);

			// Deallocate space.
			instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
			instruction->AddRegisterOperand(REG_ESP);
			instruction->AddIntLiteralOperand(funcSymbol->ParameterSize);
		}

		instruction = new Instruction(funcSymbol, OPCODE_COMMENT, _currentToken);
		instruction->AddStringLiteralOperand("\n\t; Nullify Variables");
		
		// Push in the value of this.
		thisVar->DataType->PushVariableValue(thisVar);
		
		// Nullify all variables!
		for (int i = 0; i < classSymbol->Children.size(); i++)
		{
			Symbol* sym = classSymbol->Children.at(i);
			if (sym->GetSymbolType() == ST_VARIABLE && sym->Scope == classSymbol)
			{		
				instruction = new Instruction(funcSymbol, OPCODE_COMMENT, _currentToken);
				instruction->AddStringLiteralOperand("\n\t; Null: " + sym->Identifier);
				VariableSymbol* var = ((VariableSymbol*)sym);
				var->DataType->NullifyVariable(var);
			}
		}
		
		// If there is any assignment-instructions in the class scope, add them in here.
		for (int i = 0; i < classSymbol->Instructions.size(); i++)
		{
			instruction = classSymbol->Instructions.at(i);
			bool use = true;

			// We don't want gc functions in constructor!
			if (instruction->OpCode == OPCODE_CALL)// && instruction->Operands[0]->CreateAssembly() == "_icr__ExecuteGC")
				use = false;

			if (use == true)
				_currentScope->Instructions.push_back(instruction);
		}
		classSymbol->Instructions.clear();
		
		// Pop this back out.
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(4);

		WriteFunctionPostfix(funcSymbol, true);
		_currentScope = previousScope;	
	}
}

// --------------------------------------------------------------------------------
//	Parses a continue statement in a loop.
//
//		continue;
//		continue (loop depth);
// --------------------------------------------------------------------------------
void Parser::ParseContinue()
{
	Instruction* instruction;
	int loopIndex = 0;

	// Check we are in a valid scope.
	if (_loopTrackerStack.size() == 0)
		g_app->EmitError("The continue keyword can only be used inside a loop.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
	// Parse possible loop depth.
	if (LookAheadToken().ID == TOKEN_CHAR_OPEN_PARENTHESIS)
	{
		ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
		int val = StringHelper::StringToInt(ExpectToken(TOKEN_LITERAL_INTEGER).Literal);
		ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
	
		if (val <= 0|| val > _loopTrackerStack.size())
			g_app->EmitError("Attempt to continue in loop beyond loop depth (loop depth is " + StringHelper::IntToString(_loopTrackerStack.size()) + ", loop " + StringHelper::IntToString(val) + " requested).", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
		loopIndex = val - 1;
	}

	// Break out of the loop.
	int index = _loopTrackerStack.size() - (loopIndex + 1);
	LoopTracker tracker = _loopTrackerStack.at(index);

	// Jump to loop tracker.
	if (_currentPass == 2)
	{
		// Can we continue on this?
		if (tracker.ContinueTarget == NULL)
			g_app->EmitError("The continue keyword can not be used on this statement.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
		// Jump to the start statement.
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(tracker.ContinueTarget);
	}
}

// --------------------------------------------------------------------------------
//	Parses a break statement in a loop.
//
//		break;
//		break (loop depth);
// --------------------------------------------------------------------------------
void Parser::ParseBreak()
{
	Instruction* instruction;
	int loopIndex = 0;

	// Check we are in a valid scope.
	if (_loopTrackerStack.size() == 0)
		g_app->EmitError("The break keyword can only be used inside a loop.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
	// Parse possible loop depth.
	if (LookAheadToken().ID == TOKEN_CHAR_OPEN_PARENTHESIS)
	{
		ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
		int val = StringHelper::StringToInt(ExpectToken(TOKEN_LITERAL_INTEGER).Literal);
		ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
	
		if (val <= 0 || val > _loopTrackerStack.size())
			g_app->EmitError("Attempt to break out of loop beyond loop depth (loop depth is " + StringHelper::IntToString(_loopTrackerStack.size()) + ", loop " + StringHelper::IntToString(val) + " requested).", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
		loopIndex = val - 1;
	}

	// Break out of the loop.
	int index = _loopTrackerStack.size() - (loopIndex + 1);
	LoopTracker tracker = _loopTrackerStack.at(index);

	// Jump to loop tracker.
	if (_currentPass == 2)
	{
		// Can we continue on this?
		if (tracker.BreakTarget == NULL)
			g_app->EmitError("The break keyword can not be used on this statement.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		// Jump to the start statement.
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(tracker.BreakTarget);
	}
}

// --------------------------------------------------------------------------------
//	Parses an if block.
//
//		if (Expr)
//		{ 
//		}
//		else if (Expr)
//		{
//		}
//		else
//		{
//		}
// --------------------------------------------------------------------------------
void Parser::ParseIf()
{
	Instruction* instruction;
	JumpTarget* falseJumpTarget;
	JumpTarget* skipFalseJumpTarget;

	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("The if keyword can only be used within the global scope or a function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Create a jump target.
	if (_currentPass == 2)
		falseJumpTarget = new JumpTarget(_currentToken);

	// Parse the if expression.
	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
	DataType* exprType = ParseExpression();
	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);

	// Jump to false label if expression evaluates to false.
	if (_currentPass == 2)
	{			
		// Not a boolean type? Cast to one then!
		DataType* boolDT = new BoolDataType();
		exprType->CastTo(boolDT);
		exprType = boolDT;

		// Allocate a comparison register.
		Register reg = AllocateRegister(exprType);

		// Pop the value into the register.
		exprType->Pop(reg);

		// Compare result to null.
		instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
		instruction->AddRegisterOperand(reg);
		instruction->AddIntLiteralOperand(0);

		// Deallocate a comparison register.
		DeallocateRegister(reg);

		// Jump to false jump target if the result is equal.
		instruction = new Instruction(_currentScope, OPCODE_JE, _currentToken);
		instruction->AddLabelOperand(falseJumpTarget);
	}

	// Parse the if block.
	ParseStatement();

	// Is there an else block after?
	if (!EndOfTokens() && LookAheadToken().ID == TOKEN_KEYWORD_ELSE)
	{
		ExpectToken(TOKEN_KEYWORD_ELSE);

		if (_currentPass == 2)
		{
			skipFalseJumpTarget = new JumpTarget(_currentToken);
		
			// Jump to the skip false block label.
			instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
			instruction->AddLabelOperand(skipFalseJumpTarget);
		}

		// Bind the false jump target.
		if (_currentPass == 2) falseJumpTarget->Bind(_currentScope);

		// Parse the else block.
		ParseStatement();
		
		// Bind the skip false jump target.
		if (_currentPass == 2) skipFalseJumpTarget->Bind(_currentScope);
	}
	else
	{
		// Bind the false jump target.
		if (_currentPass == 2) falseJumpTarget->Bind(_currentScope);
	}
}

// --------------------------------------------------------------------------------
//	Parses a for block.
//
//		for (statement; expr; statement)
// --------------------------------------------------------------------------------
void Parser::ParseFor()
{
	Instruction* instruction;
	JumpTarget* booleanExprJumpTarget = NULL;
	JumpTarget* incrementExprJumpTarget = NULL;
	JumpTarget* startBlockJumpTarget = NULL;
	JumpTarget* endBlockJumpTarget = NULL;

	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("The for keyword can only be used within the global scope or a function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Create jump targets.
	if (_currentPass == 2)
	{
		booleanExprJumpTarget = new JumpTarget(_currentToken);
		incrementExprJumpTarget = new JumpTarget(_currentToken);
		startBlockJumpTarget = new JumpTarget(_currentToken);
		endBlockJumpTarget = new JumpTarget(_currentToken);
	}

	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);

	// Parse the initialization statement.
	if (LookAheadToken().ID != TOKEN_CHAR_SEMICOLON)
	{
		ParseStatement();
		//ExpectToken(TOKEN_CHAR_SEMICOLON); 
	}
	else
		ExpectToken(TOKEN_CHAR_SEMICOLON); 
	
	// Bind the expression target.
	if (_currentPass == 2)
		booleanExprJumpTarget->Bind(_currentScope);

	// Parse the boolean expression.
	if (LookAheadToken().ID != TOKEN_CHAR_SEMICOLON)
	{
		// Parse the boolean expression.
		DataType* exprType = ParseExpression();

		if (_currentPass == 2)
		{
			// Not a boolean type? Cast to one then!
			DataType* boolDT = new BoolDataType();
			exprType->CastTo(boolDT);
			exprType = boolDT;

			// Allocate a comparison register.
			Register reg = AllocateRegister(exprType);

			// Pop the value into the register.
			exprType->Pop(reg);

			// Compare result to null.
			instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
			instruction->AddRegisterOperand(reg);
			instruction->AddIntLiteralOperand(0);

			// Deallocate a comparison register.
			DeallocateRegister(reg);

			// Jump to false jump target if the result is equal.
			instruction = new Instruction(_currentScope, OPCODE_JE, _currentToken);
			instruction->AddLabelOperand(endBlockJumpTarget);

			// Jump to the start statement.
			instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
			instruction->AddLabelOperand(startBlockJumpTarget);
		}

		ExpectToken(TOKEN_CHAR_SEMICOLON); 
	}
	else
		ExpectToken(TOKEN_CHAR_SEMICOLON); 

	// Bind the expression target.
	if (_currentPass == 2)
		incrementExprJumpTarget->Bind(_currentScope);

	// Parse the increment expression.
	if (LookAheadToken().ID != TOKEN_CHAR_CLOSE_PARENTHESIS)
	{
		// Parse the increment statement.
		ParseStatement();
	}

	// Jump to the boolean statement.
	if (_currentPass == 2)
	{
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(booleanExprJumpTarget);
	}

	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
	
	// Bind the start block jump target.
	if (_currentPass == 2)
		startBlockJumpTarget->Bind(_currentScope);

	// Push the tracker.
	_loopTrackerStack.push_back(LoopTracker(incrementExprJumpTarget, endBlockJumpTarget));

	// Parse the loop statement.
	ParseStatement();

	// Pop the tracker.
	_loopTrackerStack.pop_back();

	// Create the block postfix.
	if (_currentPass == 2)
	{
		// Jump to the increment statement.
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(incrementExprJumpTarget);
	}
	
	// Bind the end block jump target.
	if (_currentPass == 2)
		endBlockJumpTarget->Bind(_currentScope);
}

// --------------------------------------------------------------------------------
//	Parses a foreach block.
//
//		foreach (var in generator)
// --------------------------------------------------------------------------------
void Parser::ParseForEach()
{
	Instruction* instruction;
	JumpTarget* startJumpTarget = NULL;
	JumpTarget* endJumpTarget = NULL;

	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("The for keyword can only be used within the global scope or a function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);

	// Read in variable.
	VariableSymbol* variable = NULL;
	if (LookAheadToken().ID == TOKEN_KEYWORD_VAR)
	{
		ExpectToken(TOKEN_KEYWORD_VAR);
		variable = ParseVariable(false);
	}
	else if (LookAheadToken().ID == TOKEN_LITERAL_IDENTIFIER)
	{
		ExpectToken(TOKEN_LITERAL_IDENTIFIER);

		Symbol* namespaceScope = ParseNamespaceScope();
		std::string varName = _currentToken.Literal;

		variable = dynamic_cast<VariableSymbol*>(namespaceScope->FindChild(varName, false, true));
	}
	else
	{
		NextToken();
		g_app->EmitError("Expecting variable name or declaration but encountered unexpected token '"+_currentToken.Literal+"'.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}
	if (_currentPass == 2 && variable == NULL)
		g_app->EmitError("Attempt to iterate using undefined variable '"+_currentToken.Literal+"'.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Read in the "in" keyword.
	ExpectToken(TOKEN_KEYWORD_IN);

	// Read in the iterator.
	ExpectToken(TOKEN_LITERAL_IDENTIFIER);

	Symbol* namespaceScope = ParseNamespaceScope();
	FunctionSymbol* generatorFunction = NULL;

	// Generator call!
	if (LookAheadToken().ID == TOKEN_CHAR_OPEN_PARENTHESIS)
	{
		// Get the function symbol.
		FunctionSymbol* functionSymbol = dynamic_cast<FunctionSymbol*>(namespaceScope->FindChild(_currentToken.Literal, false, true));
		if (functionSymbol->IsGenerator == false)
			g_app->EmitError("Functions used in foreach must be generator functions!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		// Check return type is the same.
		if (functionSymbol->ReturnDataType->RequiresCast(variable->DataType) == true)
			g_app->EmitError("Foreach variable must use the same data type that the generator function returns!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		generatorFunction = functionSymbol;
	}

	// Eachin an object :3.
	else
	{
		// TODO: Variable support for generators!
		g_app->EmitError("TODO: EachIn support for objects!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}

	// Initial code. 
	// Generates a new generator context.
	if (_currentPass == 2)
	{
		startJumpTarget = new JumpTarget(_currentToken);
		endJumpTarget = new JumpTarget(_currentToken);
	
		// Push function descriptor.
		instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
		instruction->AddLabelOperand(generatorFunction, "_meta");

		// Call the generator context creation function.
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__Function_NewGeneratorContext");
		
		// Clean up the stack.
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(4);

		// Push the context into the stack.
		instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
		instruction->AddRegisterOperand(REG_EAX);

		startJumpTarget->Bind(_currentScope);

		// Parse the function call.
		ParseFunctionCall(generatorFunction, false, "", false, false);

		// Call the generator invokation function.
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__Function_CallGenerator");

		// Clean up the stack.
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(generatorFunction->ParameterSize);

		// Check return is valid! (ebx=1 for more 0 for done)
		instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
		instruction->AddRegisterOperand(REG_EBX);
		instruction->AddIntLiteralOperand(0);

		// Jump to end.
		instruction = new Instruction(_currentScope, OPCODE_JE, _currentToken);
		instruction->AddLabelOperand(endJumpTarget);

		// Assign the return to the value.
		generatorFunction->ReturnDataType->AssignToVariable(variable);
	}

	// Parse the function-call.
	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
	
	// Push the tracker.
	_loopTrackerStack.push_back(LoopTracker(startJumpTarget, endJumpTarget));

	// Parse the loop statement.
	ParseStatement();

	// Pop the tracker.
	_loopTrackerStack.pop_back();

	// Cleanup code.
	// Gets rid of generator context.
	if (_currentPass == 2)
	{
		// Jump back to the start.
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(startJumpTarget);

		endJumpTarget->Bind(_currentScope);
		
		// Free generator context.
		instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
		instruction->AddStringLiteralOperand("_icr__Function_FreeGeneratorContext");

		// Pop contet off the stack.
		instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddIntLiteralOperand(4);
	}
}

// --------------------------------------------------------------------------------
//	Parses a while block.
//
//		while (Expr)
//		{ 
//		}
// --------------------------------------------------------------------------------
void Parser::ParseWhile()
{
	Instruction* instruction;
	JumpTarget* startJumpTarget = NULL;
	JumpTarget* endJumpTarget = NULL;

	// Check we are in a valid scope.
	if (_currentScope->GetSymbolType() != ST_FUNCTION && _currentScope != _globalScope)
		g_app->EmitError("The while keyword can only be used within the global scope or a function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Create a jump target.
	if (_currentPass == 2)
	{
		startJumpTarget = new JumpTarget(_currentToken);
		endJumpTarget = new JumpTarget(_currentToken);

		startJumpTarget->Bind(_currentScope);
	}

	// Parse the if expression.
	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
	DataType* exprType = ParseExpression();
	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);

	// Pare the loop expresion.
	if (_currentPass == 2)
	{			
		// Not a boolean type? Cast to one then!
		DataType* boolDT = new BoolDataType();
		exprType->CastTo(boolDT);
		exprType = boolDT;

		// Allocate a comparison register.
		Register reg = AllocateRegister(exprType);

		// Pop the value into the register.
		exprType->Pop(reg);

		// Compare result to null.
		instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
		instruction->AddRegisterOperand(reg);
		instruction->AddIntLiteralOperand(0);

		// Deallocate a comparison register.
		DeallocateRegister(reg);

		// Jump to false jump target if the result is equal.
		instruction = new Instruction(_currentScope, OPCODE_JE, _currentToken);
		instruction->AddLabelOperand(endJumpTarget);
	}

	// Push the tracker.
	_loopTrackerStack.push_back(LoopTracker(startJumpTarget, endJumpTarget));

	// Parse the loop statement.
	ParseStatement();

	// Pop the tracker.
	_loopTrackerStack.pop_back();

	// Emit the end of the loop.
	if (_currentPass == 2)
	{
		// Jump to the start of the loop.
		instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
		instruction->AddLabelOperand(startJumpTarget);

		// Bind the end of the loop.
		endJumpTarget->Bind(_currentScope);
	}
}

// --------------------------------------------------------------------------------
//	Parses a namespace block.
//
//		namespace identifier
//		{ 
//		}
// --------------------------------------------------------------------------------
void Parser::ParseNamespace()
{
	// Check we are in a valid scope.
	if (_currentScope != _globalScope)
		g_app->EmitError("The namespace keyword can only be used within the global scope.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Lets parse the identifiers of the namespace.
	Symbol* scope = _globalScope;
	while (true)
	{
		std::string scopeName = ExpectToken(TOKEN_LITERAL_IDENTIFIER).Literal;

		// Make the scope.
		Symbol* preSymbol = scope->FindChild(scopeName, false, false);
		if (preSymbol != NULL)
		{
			if (preSymbol->GetSymbolType() != ST_NAMESPACE)
				g_app->EmitError("Attempting to override non-namespace symbol '"+scopeName+"' in namespace '"+scope->Identifier+"'.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

			scope = preSymbol;
		}
		else
		{
			scope = new NamespaceSymbol(scopeName, scope, _currentToken);
			DefineStringLiteral(scopeName, true);
		}

		// Parse next scope.
		if (LookAheadToken().ID == TOKEN_OP_SCOPE_RESOLUTION)
			ExpectToken(TOKEN_OP_SCOPE_RESOLUTION);
		else
			break;
	}

	// Parse the statement.
	Symbol* oldScope = _currentScope;
	_currentScope = scope;
	ParseBlock();
	_currentScope = oldScope;
}

// --------------------------------------------------------------------------------
//	Parses a function declaration.
//
//		function void Name(arguments...)
//		{
//		}
// --------------------------------------------------------------------------------
void Parser::ParseFunction()
{
	DataType* returnDataType = new VoidDataType();
	FunctionSymbol* functionSymbol = NULL;
	std::string identifier;
	
	std::string paramIdentifier;
	DataType* paramDataType;
	VariableSymbol* parameterSymbol;

	bool isNewMethod = false;
	bool isDeleteMethod = false;

	bool isGenerator = false;

	TokenID functionStartToken = _currentToken.ID;

	// Check we are in a valid scope.
	if (functionStartToken == TOKEN_KEYWORD_METHOD)
	{
		if (_currentScope->GetSymbolType() != ST_CLASS)
			g_app->EmitError("Methods can only be declared within a class scope.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}
	else
	{
		if (_currentScope != _globalScope && _currentScope->GetSymbolType() != ST_CLASS && _currentScope->GetSymbolType() != ST_NAMESPACE)
			g_app->EmitError("Functions can only be declared within a class scope, namespace scope or the global scope.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}

	// Read in dat generator.
	if (LookAheadToken().ID == TOKEN_KEYWORD_GENERATOR)
	{
		ExpectToken(TOKEN_KEYWORD_GENERATOR);
		isGenerator = true;
	}

	// Read in the data type.
	if (LookAheadToken(2).ID != TOKEN_CHAR_OPEN_PARENTHESIS)
		returnDataType = ParseDataType();

	// Read in the identifier.
	Token lat = LookAheadToken();
	if (lat.ID == TOKEN_KEYWORD_NEW)
	{
		isNewMethod = true;
		NextToken();

		if (functionStartToken != TOKEN_KEYWORD_METHOD)		
			g_app->EmitError("New method can only be declared as a method of a class!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);	

		if (returnDataType->GetPrimitive() != PDT_VOID)		
			g_app->EmitError("New method can not provide a return value.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);	
	}
	else if (lat.ID == TOKEN_KEYWORD_DELETE)
	{
		isDeleteMethod = true;
		NextToken();

		if (functionStartToken != TOKEN_KEYWORD_METHOD)		
			g_app->EmitError("Delete method can only be declared as a method of a class!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);	
		
		if (returnDataType->GetPrimitive() != PDT_VOID)		
			g_app->EmitError("Delete method can not provide a return value.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);	
	}
	else
		ExpectToken(TOKEN_LITERAL_IDENTIFIER);

	identifier = _currentToken.Literal;

	// Work out parameter count.
	int parameterCount = CalcFunctionParameters();
	if (functionStartToken == TOKEN_KEYWORD_METHOD)
		parameterCount += 1;

	// If we are in pass one, then create the symbol.
	if (_currentPass == 1)
	{
		// Check no other symbols exist with the same name.
		if (_currentScope->FindChildFunction(identifier, parameterCount, false, false) != NULL)
			g_app->EmitError("Redefinition of identifier \"" + identifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		// Create the function symbol.
		functionSymbol = new FunctionSymbol(identifier, _currentScope, _currentToken);
		functionSymbol->ReturnDataType = returnDataType;
		functionSymbol->ParameterCount = 0;
		functionSymbol->ParameterSize = 0;
		functionSymbol->LocalSize = 0;
		functionSymbol->LocalCount = 0;
		functionSymbol->IsExtern = (_externBlockDepth > 0 ? true : false);
		functionSymbol->IsMethod = (functionStartToken == TOKEN_KEYWORD_METHOD);
		functionSymbol->IsGenerator = isGenerator;
		functionSymbol->AccessModifier = _currentAccessModifier;

		if (functionSymbol->IsExtern == true && functionSymbol->IsGenerator == true)
			g_app->EmitError("Generator can not be declared as external.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		// Define the name and token paths as string symbols for the function definitions.
		DefineStringLiteral(functionSymbol->Identifier, true);
		DefineStringLiteral(functionSymbol->DefinitionToken.Path, true);
	}

	// If we are in pass two, then update the symbol. 
	else if (_currentPass == 2)
	{
		functionSymbol = (FunctionSymbol*)_currentScope->FindChildFunction(identifier, parameterCount, false, false);
		functionSymbol->ReturnDataType = returnDataType;
	}

	// Read in the parameter list.
	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
	if (LookAheadToken().ID != TOKEN_CHAR_CLOSE_PARENTHESIS)
	{
		// Delete takes no arguments :(
		if (isDeleteMethod == true)
			g_app->EmitError("Delete method can not take any arguments!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
		
		// New takes no arguments (for now)
	//	if (isNewMethod == true)
	//		g_app->EmitError("New method can not take any arguments (YET, TODO)!", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

		while (true)
		{
			// Read in the data type.
			paramDataType = ParseDataType();

			// Read in the identifier.
			paramIdentifier = ExpectToken(TOKEN_LITERAL_IDENTIFIER).Literal;

			// If in pass one, create the symbol.
			if (_currentPass == 1)
			{
				// Check no other symbols exist with the same name.
				if (functionSymbol->FindChild(paramIdentifier, false, false) != NULL)
					g_app->EmitError("Redefinition of parameter \"" + paramIdentifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

				parameterSymbol = new VariableSymbol(paramIdentifier, functionSymbol, _currentToken);
				parameterSymbol->DataType = paramDataType;
				parameterSymbol->VariableType = VT_PARAMETER;

				// Increment the local count because we are going to store the parameter as a local
				// so the storage size is correct (as some values get promoted when passed).
				//functionSymbol->LocalCount++;
				//functionSymbol->LocalSize += paramDataType->GetStoredSize();

				// Increment the parameter count.
				functionSymbol->ParameterCount++;
				functionSymbol->ParameterSize += paramDataType->GetSize();
				
				// Define the name and token paths as string symbols for the function definitions.
				DefineStringLiteral(parameterSymbol->Identifier, true);
				DefineStringLiteral(parameterSymbol->DefinitionToken.Path, true);
				//parameterSymbol->StackOffset = (functionSymbol->ParameterSize);
			}

			// If in pass two, update the symbol.
			else if (_currentPass == 2)
			{
				parameterSymbol = (VariableSymbol*)functionSymbol->FindChild(paramIdentifier);
				parameterSymbol->DataType = paramDataType;
			}

			// Is there another parameter.
			if (LookAheadToken(1).ID == TOKEN_CHAR_COMMA)
				ExpectToken(TOKEN_CHAR_COMMA);
			else
				break;
		}
	}
	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
	
	// Define the "this" parameter (hidden last parameter for methods).
	if (functionStartToken == TOKEN_KEYWORD_METHOD && _currentPass == 1)
	{
		parameterSymbol = new VariableSymbol("this", functionSymbol, _currentToken);
		parameterSymbol->DataType = new ClassDataType((ClassSymbol*)_currentScope);
		parameterSymbol->VariableType = VT_PARAMETER;
		parameterSymbol->IsConst = true;

		functionSymbol->ParameterCount++;
		functionSymbol->ParameterSize += parameterSymbol->DataType->GetSize();
	}

	// Go through all local variables and parameters and calculate their stack offsets.
	if (_currentPass == 2)
	{
		int localOffset = 0;
		int paramOffset = 4;

		for (int i = 0; i < (int)functionSymbol->Children.size(); i++)
		{
			Symbol* symbol = functionSymbol->Children.at(i);
			if (symbol->GetSymbolType() == ST_VARIABLE)
			{
				VariableSymbol* variable = (VariableSymbol*)symbol;
				if (variable->VariableType == VT_LOCAL)
				{
					localOffset += variable->DataType->GetSize();
					variable->StackOffset = -localOffset;
				}
				else if (variable->VariableType == VT_PARAMETER)
				{
					paramOffset += variable->DataType->GetSize();
					//variable->ParamStackOffset = (paramOffset);
					

					variable->StackOffset = (paramOffset);
					
					//printf("PARAM: %s -> %i\n", variable->Identifier.c_str(), variable->StackOffset);

					// Create a local for parameter too.
					//localOffset += variable->DataType->GetStoredSize();
					//variable->StackOffset = -localOffset;
				}
			}
		}
	}

	if (functionSymbol->IsExtern == true)
	{
		ExpectToken(TOKEN_CHAR_SEMICOLON);
	}
	else
	{
		WriteFunctionPrefix(functionSymbol);

		// Parse the instruction block.
		Symbol* previousScope = _currentScope;
		_currentScope = functionSymbol;
		ParseBlock();
		_currentScope = previousScope;
		
		WriteFunctionPostfix(functionSymbol, true);

		// Check a return value exists.
	//	if (_currentPass == 2 && functionSymbol->ReturnDataType->GetPrimitive() != PDT_VOID && functionSymbol->ReturnExists != true)
	//		g_app->EmitError("Function '" + functionSymbol->Identifier + "' expects return, but no return given.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}
}

// --------------------------------------------------------------------------------
//	Calculates the number of functions in a function definition.
// --------------------------------------------------------------------------------
int Parser::CalcFunctionParameters()
{
	int paramCount = 0;
	int lastTokenIndex = _tokenIndex;

	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
	if (LookAheadToken().ID != TOKEN_CHAR_CLOSE_PARENTHESIS)
	{
		while (true)
		{
			// Read in the data type.
			ParseDataType();

			// Read in the identifier.
			ExpectToken(TOKEN_LITERAL_IDENTIFIER);

			// Increment parameter count!
			paramCount++;

			// Is there another parameter.
			if (LookAheadToken(1).ID == TOKEN_CHAR_COMMA)
				ExpectToken(TOKEN_CHAR_COMMA);
			else
				break;
		}
	}

	// Reset token stream to end of parameters.
	_tokenIndex = lastTokenIndex - 1;
	NextToken();

	return paramCount;
}

// --------------------------------------------------------------------------------
//	Calculates the number of functions in a function calls.
// --------------------------------------------------------------------------------
int Parser::CalcFunctionCallParameters()
{
	int paramCount = 0;
	int lastTokenIndex = _tokenIndex;
	
	// Read in the opening parenthesis.
	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);

	while (LookAheadToken().ID != TOKEN_CHAR_CLOSE_PARENTHESIS)
	{
		int oldPass = _currentPass;
		_currentPass = -99; // Stop instructions being generated.
		DataType* exprType = ParseExpression();
		_currentPass = oldPass;

		// Increment parameter count.
		paramCount++;

		// Another parameter available?
		if (LookAheadToken().ID == TOKEN_CHAR_COMMA)
			ExpectToken(TOKEN_CHAR_COMMA);
		else
			break;
	}
		
	// Read in the closing parenthesis.
	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);

	// Reset token stream to end of parameters.
	_tokenIndex = lastTokenIndex - 1;
	NextToken();

	return paramCount;
}

// --------------------------------------------------------------------------------
//	Parsea a function call. It takes the asseumtion that the identifier has
//  already been read.
// --------------------------------------------------------------------------------
DataType* Parser::ParseFunctionCall(FunctionSymbol* explicitSymbol, bool doNotUseVirtual, std::string name, bool isMethod, bool doCall)
{
	Instruction* instruction;


	// Store the function's identifier for later.
	std::string functionIdentifier = _currentToken.Literal;
	if (name != "")
		functionIdentifier = name;

	// Read in the opening parenthesis.
	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);

	// Read in / skip all of the parameters.
	int paramCount = 0;
	std::vector<int> tokenIndexesOfParameters;
	while (LookAheadToken().ID != TOKEN_CHAR_CLOSE_PARENTHESIS)
	{
		// Parse the expression.
		tokenIndexesOfParameters.push_back(_tokenIndex);
		
		int oldPass = _currentPass;
		_currentPass = -99; // Stop instructions being generated.
		DataType* exprType = ParseExpression();
		_currentPass = oldPass;

		// Increment parameter count.
		paramCount++;
		
		// Validate parametrer.
		if (_currentPass == 2)
		{
			// Check parameter count is correct.
			//if (paramCount > symbol->ParameterCount)
			//	g_app->EmitError("Incorrect number of parameters passed to function call \"" + functionIdentifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

			// Check expression type is correct. (Don't do this anymore, dealt with in casting below).
			//VariableSymbol* paramSymbol = (VariableSymbol*)symbol->Children.at(paramCount - 1);
			//if (paramSymbol->DataType->GetPrimitive() != exprType->GetPrimitive())
			//	g_app->EmitError("Incorrect data type for parameter "+StringHelper::IntToString(paramCount)+" in function call to \"" + functionIdentifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
		}

		// Another parameter available?
		if (LookAheadToken().ID == TOKEN_CHAR_COMMA)
			ExpectToken(TOKEN_CHAR_COMMA);
		else
			break;
	}
	if (isMethod == true) 
		paramCount++;

	// Check function exists.
	FunctionSymbol* symbol = explicitSymbol;
	if (_currentPass == 2 && symbol == NULL)
	{
		symbol = (FunctionSymbol*)_currentScope->FindChildFunction(functionIdentifier, paramCount);
		if (symbol == NULL)
			symbol = (FunctionSymbol*)_currentScope->FindChildFunction(functionIdentifier, paramCount + 1);
		if (symbol == NULL)
			g_app->EmitError("Undeclared identifier \"" + functionIdentifier + "\" could not be found for function call.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}

	// No calley the generators!
	if (_currentPass == 2 && symbol->IsGenerator == true)
		g_app->EmitError("Generators can only be called using an eachin keyword.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
	// Check we are in a valid scope.
	if (_currentScope != _globalScope && _currentScope->GetSymbolType() != ST_FUNCTION)
		g_app->EmitError("Function's can only be called within a function, method or the global scope.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	
	// Check parameter count is correct.
	if (_currentPass == 2 && paramCount > symbol->ParameterCount)
		g_app->EmitError("Incorrect number of parameters passed to function call \"" + functionIdentifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	// Check parameter count is correct.
	if (_currentPass == 2 && symbol->IsMethod == true)
	{
		if (_currentPass == 2 && paramCount != symbol->ParameterCount)
			g_app->EmitError("Incorrect number of parameters passed to function call \"" + functionIdentifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}
	else
	{
		if (_currentPass == 2 && paramCount != symbol->ParameterCount)
			g_app->EmitError("Incorrect number of parameters passed to function call \"" + functionIdentifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}

	// Read in all of the parameters in reverse order.
	// We do this because the c calling convention requires parameters to
	// be passed from right to left.
	if (_currentPass == 2)
	{
		int lastTokenIndex = _tokenIndex;

		for (int i = (int)tokenIndexesOfParameters.size() - 1; i >= 0; i--)
		{
			int index = tokenIndexesOfParameters.at(i);

			// Start parsing from the start index.
			_tokenIndex = index - 1;
			NextToken();

			// Parse the expression and generate the instructions.
			DataType* returnType = ParseExpression();
			DataType* expectedType = ((VariableSymbol*)symbol->Children[i])->DataType;

			// Cast to correct type
			returnType->CastTo(expectedType);
		}
		
		// Reset token stream to end of parameters.
		_tokenIndex = lastTokenIndex - 1;
		NextToken();
	}

	// Read in the closing parenthesis.
	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);

	// Create invokation instructions.
	if (_currentPass == 2)
	{			
		if (symbol->IsMethod == true && doNotUseVirtual == false)
		{
			// Allocate register for "this".
			Register funcReg = AllocateRegister(new IntDataType());
		
			// Load in the value of this.
			instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
			instruction->AddRegisterOperand(funcReg);
			instruction->AddAddressOffsetOperand(new RegisterOperand(REG_ESP), new IntegerLiteralOperand(symbol->ParameterSize - 4), new IntDataType());

			// Load the class definition.
			instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
			instruction->AddRegisterOperand(funcReg);
			instruction->AddAddressOffsetOperand(new RegisterOperand(funcReg), new IntegerLiteralOperand(-(ClassHeaderSize)), new IntDataType());
			
			// Load the vf-table.
			instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
			instruction->AddRegisterOperand(funcReg);
//			instruction->AddAddressOffsetOperand(new RegisterOperand(thisReg), new IntegerLiteralOperand(((-ClassHeaderSize) + symbol->VirtualFunctionTableOffset) - 8), new IntDataType());
			instruction->AddAddressOffsetOperand(new RegisterOperand(funcReg), new IntegerLiteralOperand(VFTABLE_CLASS_OFFSET), new IntDataType());

			// Load address into register.
		////	instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
		//	instruction->AddRegisterOperand(funcReg);
		//	instruction->AddAddressOffsetOperand(new RegisterOperand(thisReg), new IntegerLiteralOperand((-ClassHeaderSize) + symbol->VirtualFunctionTableOffset), new IntDataType());

			// Invoke method.
			if (doCall == true)
			{
				instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
				instruction->AddAddressOffsetOperand(new RegisterOperand(funcReg), new IntegerLiteralOperand(symbol->VirtualFunctionTableOffset * 4), new IntDataType());
				//instruction->AddRegisterOperand(funcReg);
			}

			// Deallocate things :3.
			DeallocateRegister(funcReg);
		}
		else
		{
			//if (doNotUseVirtual == true)
			//{
			//}

			// Invoke function.
			if (doCall == true)
			{
				instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
				instruction->AddLabelOperand(symbol);
			}
		}

		// Cleanup Stack
		if (symbol->ParameterSize != 0 && doCall == true)
		{
			instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
			instruction->AddRegisterOperand(REG_ESP);
			instruction->AddIntLiteralOperand(symbol->ParameterSize);
		}

		// Push return value.
		//instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToekGn);
		//instruction->AddRegisterOperand(REG_EAX);

		/*
		// If external, call the c function.
		if (symbol->IsExtern == true)
		{
			// Invoke function.
			instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
			instruction->AddLabelOperand(symbol);

			// Cleanup Stack
			instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
			instruction->AddRegisterOperand(REG_ESP);
			instruction->AddIntLiteralOperand(symbol->ParameterSize);

			// Push return value.
			instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
			instruction->AddRegisterOperand(REG_EAX);
		}

		// If not, call the source function.
		else
		{

		}
		*/
	}

	if (_currentPass == 2)
		return symbol->ReturnDataType;
	else
		return new VoidDataType();
}

// --------------------------------------------------------------------------------
//	Returns a memory access operand used to access a variables value.
// --------------------------------------------------------------------------------
Operand* Parser::GetVariableAccessOperand(VariableSymbol* variable, int offset, Register thisRegister)
{
	switch (variable->VariableType)
	{
		case VT_FIELD:
			{	
				if (offset != 0)
				{
					return new AddressOffsetOperand(new RegisterOperand(thisRegister), new IntegerLiteralOperand(variable->ClassOffset + offset), variable->DataType);
				}
				else
				{
					return new AddressOffsetOperand(new RegisterOperand(thisRegister), new IntegerLiteralOperand(variable->ClassOffset), variable->DataType);
				}
			}
		case VT_GLOBAL:
		case VT_CONST:
			{
				if (offset != 0)
				{
					return new AddressOffsetOperand(new LabelOperand(variable), new IntegerLiteralOperand(offset), variable->DataType);
				}
				else
				{
					return new AddressOperand(new LabelOperand(variable), variable->DataType);
				}
			}
		case VT_LOCAL:
			{
				return new AddressOffsetOperand(new RegisterOperand(REG_EBP), new IntegerLiteralOperand(variable->StackOffset + offset), variable->DataType);
			}
		case VT_PARAMETER:
			{
				// Parameter 1: ebp - 4
				// Parameter 2: ebp - 8
				// Parameter 3: ebp - 12
				// Local 1: ebp - 16
				// Local 2: ebp - 20
				// Local 3: ebp - 24
				if (variable->UseParamStackOffset == true)
					return new AddressOffsetOperand(new RegisterOperand(REG_EBP), new IntegerLiteralOperand(variable->ParamStackOffset + offset), variable->DataType);
				else
					return new AddressOffsetOperand(new RegisterOperand(REG_EBP), new IntegerLiteralOperand(variable->StackOffset + offset), variable->DataType);
			}
	}
	return NULL;
}

// --------------------------------------------------------------------------------
//	Adds the function prefix instructions to the current scope.
// --------------------------------------------------------------------------------
void Parser::WriteFunctionPrefix(FunctionSymbol* functionSymbol)
{
	Instruction* instruction;

	Symbol* oldInstrScope = _overrideInstructionScope;
	_overrideInstructionScope = functionSymbol;

	// Add function prefix.
	if (_currentPass == 2)
	{
		instruction = new Instruction(functionSymbol, OPCODE_COMMENT, _currentToken);
		instruction->AddStringLiteralOperand("\n\t; Create Function Stack Frame");
		
		if (functionSymbol != _globalScope)
		{
			// Write out the offsets for each parameter and local.
			instruction = new Instruction(functionSymbol, OPCODE_COMMENT, _currentToken);
			instruction->AddStringLiteralOperand("Stack Layout:");

			for (int i = 0; i < (int)functionSymbol->Children.size(); i++)
			{
				Symbol* symbol = functionSymbol->Children.at(i);
				if (symbol->GetSymbolType() == ST_VARIABLE)
				{
					instruction = new Instruction(functionSymbol, OPCODE_COMMENT, _currentToken);
					instruction->AddStringLiteralOperand("\t" + symbol->Identifier + " = "+StringHelper::IntToString(((VariableSymbol*)symbol)->StackOffset));
				}
			}
		}

		// Push frame pointer on to of stack.
		instruction = new Instruction(functionSymbol, OPCODE_PUSH, _currentToken);
		instruction->AddRegisterOperand(REG_EBP);
		
		// Set frame pointer to stack pointer.
		instruction = new Instruction(functionSymbol, OPCODE_MOV, _currentToken);
		instruction->AddRegisterOperand(REG_EBP);
		instruction->AddRegisterOperand(REG_ESP);

		// Add space for our locals and arguments.
		instruction = new Instruction(functionSymbol, OPCODE_SUB, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
	//	instruction->AddIntLiteralOperand(functionSymbol->ParameterSize + functionSymbol->LocalSize);
		instruction->AddIntLiteralOperand(functionSymbol->LocalSize);
			
		// Push all registers.
		instruction = new Instruction(functionSymbol, OPCODE_PUSHAD, _currentToken);

		if (functionSymbol != _globalScope)
		{
			instruction = new Instruction(functionSymbol, OPCODE_COMMENT, _currentToken);
			instruction->AddStringLiteralOperand("\n\t; Nullify Local Variables / Increment References / Move Parameters To Local Space");

			// Go through all local variables and 0 out their memory.
			for (int i = 0; i < (int)functionSymbol->Children.size(); i++)
			{
				Symbol* symbol = functionSymbol->Children.at(i);
				if (symbol->GetSymbolType() == ST_VARIABLE)
				{
					VariableSymbol* variable = (VariableSymbol*)symbol;
					if (variable->VariableType == VT_PARAMETER)
					{
						// Increment ref counts of class references.
						if (variable->DataType->GetPrimitive() == PDT_CLASS)
							((ClassDataType*)variable->DataType)->IncrementVariableRef(variable);

						// Copy value of parameter to its "local" storage area which is where we will actually be 
						// accessing it from.
						//variable->UseParamStackOffset = true;
						//variable->DataType->PushVariableValue(variable);
						//variable->UseParamStackOffset = false;
						//variable->DataType->AssignToVariable(variable);
					}
					else if (variable->VariableType == VT_LOCAL)
					{
						// Nullify the local variable.
						variable->DataType->NullifyVariable(variable);

						//instruction = new Instruction(functionSymbol, OPCODE_MOV, _currentToken);
						//instruction->AddOperand(GetVariableAccessOperand(variable));
						//instruction->AddIntLiteralOperand(0);
					}
				}
			}
		}

		instruction = new Instruction(functionSymbol, OPCODE_COMMENT, _currentToken);
		instruction->AddStringLiteralOperand("\n\t; Function Body");
	}

	_overrideInstructionScope = oldInstrScope;
}

// --------------------------------------------------------------------------------
//	Adds the function postfix instructions to the current scope.
// --------------------------------------------------------------------------------
void Parser::WriteFunctionPostfix(FunctionSymbol* functionSymbol, bool finalReturn)
{
	Instruction* instruction;
	
	Symbol* oldInstrScope = _overrideInstructionScope;
	_overrideInstructionScope = functionSymbol;

	// Add function postfix.
	if (_currentPass == 2)
	{
		// If this is the dispose method we need to make sure we call the super-class disposal too!
		if (functionSymbol->IsMethod == true && functionSymbol->DefinitionToken.ID == TOKEN_KEYWORD_DELETE)
		{	
			ClassSymbol* classSymbol = dynamic_cast<ClassSymbol*>(functionSymbol->Scope);
			VariableSymbol* thisVar = dynamic_cast<VariableSymbol*>(functionSymbol->Children.at(functionSymbol->Children.size() - 1));
		
			// Call super-construct dispose method.
			if (classSymbol->SuperClass != NULL)
			{		
				FunctionSymbol* superDisposeFunc = dynamic_cast<FunctionSymbol*>(classSymbol->SuperClass->FindChild("delete"));

				// Push in the value of this.
				Symbol* oldScope = _currentScope;
				_currentScope = functionSymbol;
				thisVar->DataType->PushVariableValue(thisVar);
				_currentScope = oldScope;

				// Invoke function.
				instruction = new Instruction(functionSymbol, OPCODE_CALL, _currentToken);
				instruction->AddLabelOperand(superDisposeFunc);

				// Deallocate space.
				instruction = new Instruction(functionSymbol, OPCODE_ADD, _currentToken);
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(superDisposeFunc->ParameterSize);
			}
		}

		if (functionSymbol != _globalScope)
		{
			instruction = new Instruction(functionSymbol, OPCODE_COMMENT, _currentToken);
			instruction->AddStringLiteralOperand("\n\t; Decrement References");

			// Go through all parameters and decrement references.
			for (int i = 0; i < (int)functionSymbol->Children.size(); i++)
			{
				Symbol* symbol = functionSymbol->Children.at(i);
				if (symbol->GetSymbolType() == ST_VARIABLE)
				{
					VariableSymbol* variable = (VariableSymbol*)symbol;
					if (variable->VariableType == VT_PARAMETER)
					{
						if (variable->DataType->GetPrimitive() == PDT_CLASS)
						{
							((ClassDataType*)variable->DataType)->DecrementVariableRef(variable);
						}
					}
				}
			}
		}

		// If this is the final return, then nullify the return register.
		if (finalReturn == true && 
			functionSymbol->ReturnDataType->GetPrimitive() != PDT_VOID)
		{
			functionSymbol->ReturnDataType->PushDefault();
			functionSymbol->ReturnDataType->AssignToReturn();
		}
		
		instruction = new Instruction(functionSymbol, OPCODE_COMMENT, _currentToken);
		instruction->AddStringLiteralOperand("\n\t; Destroy Function Stack Frame");
		
		// Move return value into registers on the stack.
		instruction = new Instruction(functionSymbol, OPCODE_MOV, _currentToken);
		instruction->AddAddressOffsetOperand(REG_EBP, -(4 + functionSymbol->LocalSize), new IntDataType());
		instruction->AddRegisterOperand(REG_EAX);

		// Push all registers.
		instruction = new Instruction(functionSymbol, OPCODE_POPAD, _currentToken);

		// Set stack pointer to frame pointer.
		instruction = new Instruction(functionSymbol, OPCODE_MOV, _currentToken);
		instruction->AddRegisterOperand(REG_ESP);
		instruction->AddRegisterOperand(REG_EBP);

		// Pop frame pointer out of stack.
		instruction = new Instruction(functionSymbol, OPCODE_POP, _currentToken);
		instruction->AddRegisterOperand(REG_EBP);

		// Add a return instruction to the end.
		instruction = new Instruction(functionSymbol, OPCODE_RETURN, _currentToken);
	}

	_overrideInstructionScope = oldInstrScope;
}

// --------------------------------------------------------------------------------
//	Parses a block of statements surrounded by braces.
//
//		{
//			statements ...
//		}
// --------------------------------------------------------------------------------
void Parser::ParseBlock()
{
	// If we have not already read the opening brace, then read it now!
	if (_currentToken.ID != TOKEN_CHAR_OPEN_BRACE)
		ExpectToken(TOKEN_CHAR_OPEN_BRACE);

	// Is it an empty block?
	if (LookAheadToken().ID == TOKEN_CHAR_CLOSE_BRACE)
	{
		ExpectToken(TOKEN_CHAR_CLOSE_BRACE);
		return;
	}

	// Keep reading statements until we get to the end of the block.
	while (LookAheadToken().ID != TOKEN_CHAR_CLOSE_BRACE)
	{
		if (EndOfTokens() == true)
		{
			g_app->EmitError("Unexpected end of file, expecting statement block end.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
			return;
		}
		ParseStatement();
	}

	// Read in closing brace.
	ExpectToken(TOKEN_CHAR_CLOSE_BRACE);
}

// --------------------------------------------------------------------------------
//	Skips a block of statements surrounded by a brace.
//
//		{
//			statements ...
//		}
// --------------------------------------------------------------------------------
void Parser::SkipBlock()
{
	int blockDepth = 1;

	// If we have not already read the opening brace, then read it now!
	if (_currentToken.ID != TOKEN_CHAR_OPEN_BRACE)
		ExpectToken(TOKEN_CHAR_OPEN_BRACE);

	// Is it an empty block?
	if (LookAheadToken().ID == TOKEN_CHAR_CLOSE_BRACE)
	{
		ExpectToken(TOKEN_CHAR_CLOSE_BRACE);
		return;
	}

	// Keep reading statements until we get to the end of the block.
	while (blockDepth > 0)
	{
		if (EndOfTokens() == true)
		{
			g_app->EmitError("Unexpected end of file, expecting statement block end.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
			return;
		}
		
		switch (NextToken().ID)
		{
			case TOKEN_CHAR_CLOSE_BRACE:
				{
					blockDepth--;
					break;
				}
			case TOKEN_CHAR_OPEN_BRACE:
				{
					blockDepth++;
					break;
				}
			default:
				// Tokens to skip.
				break;
		}

	}
}

// --------------------------------------------------------------------------------
//	Parses a data type value.
//
//		void / int / short / class / etc
// --------------------------------------------------------------------------------
DataType* Parser::ParseDataType(bool returnNullOnError)
{
	DataType* dataType;

	// Signed or unsigned, data type prefixes.
	//if (LookAheadToken().ID == TOKEN_KEYWORD_SIGNED)
	//{
	//	dataType.IsSigned = true;
	//	NextToken();
	//}
	//if (LookAheadToken().ID == TOKEN_KEYWORD_UNSIGNED)
	//{
	//	dataType.IsSigned = false;
	//	NextToken();
	//}

	// Primitive data types?
	switch (NextToken().ID)
	{
		case TOKEN_KEYWORD_DOUBLE:
			{	
				dataType = new DoubleDataType();
				break;
			} 
		case TOKEN_KEYWORD_FLOAT:
			{	
				dataType = new FloatDataType();
				break;
			}  
		case TOKEN_KEYWORD_INT:
			{	
				dataType = new IntDataType();
				break;
			}     
		case TOKEN_KEYWORD_SHORT:
			{	
				dataType = new ShortDataType();
				break;
			}  
		case TOKEN_KEYWORD_LONG:
			{	
				dataType = new LongDataType();
				break;
			}       
		case TOKEN_KEYWORD_BYTE:
			{	
				dataType = new ByteDataType();
				break;
			}   
		case TOKEN_KEYWORD_VOID:
			{	
				dataType = new VoidDataType();
				break;
			}
		case TOKEN_KEYWORD_BOOL:
			{	
				dataType = new BoolDataType();
				break;
			} 
		//case TOKEN_KEYWORD_STRING:
		//	{	
		//		dataType = new StringDataType();
		//		break;
		//	} 
		case TOKEN_LITERAL_IDENTIFIER:
			{	
				if (StringHelper::ToLower(_currentToken.Literal) == "string")
					return new StringDataType();

				Symbol* namespaceScope = ParseNamespaceScope();
				Symbol* symbol = namespaceScope->FindChild(_currentToken.Literal, false, true);
							
				// Check its a class!
				if (symbol != NULL && symbol->GetSymbolType() != ST_CLASS)
					symbol = NULL;

				if (_currentPass == 1 && returnNullOnError == true)
					return NULL;

				if (_currentPass == 2 && symbol == NULL)
				{
					if (returnNullOnError == true)
						return NULL;
					g_app->EmitError("Undefined identifier \"" + _currentToken.Literal + "\", expecting data type.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				}

				if (_currentPass == 2 && symbol->GetSymbolType() != ST_CLASS)
				{
					if (returnNullOnError == true)
						return NULL;

					g_app->EmitError("Expecting class identifier, but encountered invalid identifier \"" + _currentToken.Literal + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				}

				ClassDataType* classDataType = new ClassDataType((ClassSymbol*)symbol);
				classDataType->ClassName = _currentToken.Literal;
				dataType = classDataType;

				break;
			}
		default:
			{	
				if (returnNullOnError == true)
					return NULL;

				g_app->EmitError("Unexpected token \"" + _currentToken.Literal + "\", expecting data type.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				break;
			}
	}

	return dataType;
}

// --------------------------------------------------------------------------------
//	Parses a namespace access.
//
//		This::Is::A::Test.???
// --------------------------------------------------------------------------------
Symbol* Parser::ParseNamespaceScope()
{
	Symbol* scope = _currentScope;
	if (LookAheadToken().ID != TOKEN_OP_SCOPE_RESOLUTION)
		return scope;

	while (true)
	{
		if (_currentPass == 2)
		{
			Symbol* name = scope->FindChild(_currentToken.Literal, false, true);
			if (name == NULL || (name->GetSymbolType() != ST_NAMESPACE && name->GetSymbolType() != ST_CLASS))
				if (scope == _currentScope)
					g_app->EmitError("Attempt to resolve unidentifier scope '"+_currentToken.Literal+"'.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				else
					break;

			scope = name;
		}

		if (LookAheadToken().ID == TOKEN_OP_SCOPE_RESOLUTION)
		{
			ExpectToken(TOKEN_OP_SCOPE_RESOLUTION);
			ExpectToken(TOKEN_LITERAL_IDENTIFIER);
		}
		else
			break;

	}
	return scope;
}

// --------------------------------------------------------------------------------
//	Converts 32bit registers to 8 and 16 bit registers.
// --------------------------------------------------------------------------------
Register Parser::Register32BitTo8Bit(Register reg)
{
	return (Register)((int)reg - 16);
}
Register Parser::Register32BitTo16Bit(Register reg)
{
	return (Register)((int)reg - 8);
}

// --------------------------------------------------------------------------------
//	If given a 32bit register it returns the other 32bit register used as its 
//  twin.
// --------------------------------------------------------------------------------
Register Parser::Register64LoBitsTwin(Register reg)
{
//EDX:EAX
//ECX:EBX
	switch (reg)
	{
		case REG_EAX: return REG_EAX;
		case REG_EBX: return REG_EBX;
		case REG_ECX: return REG_EAX;
		case REG_EDX: return REG_EBX;
	}
	g_app->EmitError("Unable to get hi-bits twin of register.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);			
}
Register Parser::Register64HiBitsTwin(Register reg)
{
	switch (reg)
	{
		case REG_EAX: return REG_EDX;
		case REG_EBX: return REG_ECX;
		case REG_ECX: return REG_ECX;
		case REG_EDX: return REG_EDX;
	}
	g_app->EmitError("Unable to get hi-bits twin of register.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);			
}

// --------------------------------------------------------------------------------
//	Returns a general purpose register that the given data type can fit in.
// --------------------------------------------------------------------------------
Register Parser::AllocateRegister(DataType* type)
{
	Register startIndex = (Register)0;
	Register endIndex = (Register)0;

	//printf("\nAllocating Register ...\n");

	// If its a long value, we need to allocate 2 registers.
	if (type->GetPrimitive() == PDT_LONG || type->GetPrimitive() == PDT_DOUBLE)
	{
		DataType* intType = new IntDataType();
		Register newReg = AllocateRegister(intType);

		if (newReg != Register64LoBitsTwin(newReg))
			SetRegisterAllocation(Register64LoBitsTwin(newReg), true);
		if (newReg != Register64HiBitsTwin(newReg))
			SetRegisterAllocation(Register64HiBitsTwin(newReg), true);

		_registerAllocationDataType[(int)Register64LoBitsTwin(newReg)] = type;
		_registerAllocationDataType[(int)Register64HiBitsTwin(newReg)] = type;

		return Register64LoBitsTwin(newReg);
	}

	// Work out the range of registers to use for the given data type.
	switch (type->GetPrimitive())
	{
		case PDT_BOOL:		startIndex = REG_EBX; endIndex = REG_EDX; break;
		case PDT_BYTE:		startIndex = REG_EBX; endIndex = REG_EDX; break;
		case PDT_SHORT:		startIndex = REG_EBX; endIndex = REG_EDX; break;
		case PDT_INT:		startIndex = REG_EBX; endIndex = REG_EDX; break;
		case PDT_LONG:		startIndex = REG_EBX; endIndex = REG_EDX; break;
		case PDT_STRING:	startIndex = REG_EBX; endIndex = REG_EDX; break;

		case PDT_FLOAT:		startIndex = REG_EBX; endIndex = REG_EDX; break;
		case PDT_DOUBLE:	startIndex = REG_EBX; endIndex = REG_EDX; break;
		case PDT_VOID:		
			{
				g_app->EmitError("Internal error, attempt to allocate register for invalid data type.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				return (Register)0;
			}
		case PDT_CLASS:		startIndex = REG_EBX; endIndex = REG_EDX; break;
	}

	// ERROR: Might overwrite return values!?

	// Allocate a register.
	for (int i = (int)startIndex; i <= (int)endIndex; i++)
	{
		if (_registerAllocation[i] == false)
		{
			//printf(("+Allocated register "+StringHelper::IntToString(i)+" to true\n").c_str());
			_registerAllocation[i] = true;
			_registerAllocationDataType[i] = type; 
			return (Register)i;
		}
	}

	// ERROR, no register :(
	g_app->EmitError("Internal error, attempt to allocate register for data type, but no registers remain.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	return (Register)0;
}

// --------------------------------------------------------------------------------
//	Allocates a given register.
// --------------------------------------------------------------------------------
void Parser::SetRegisterAllocation(Register reg, bool val) 
{ 
	
	//if (val == true)
	//	printf(("++Allocated register "+StringHelper::IntToString(reg)+" to true\n").c_str());		
	//else
	//	printf(("--Deallocated register "+StringHelper::IntToString(reg)+" to false\n").c_str());


	if (val == true && _registerAllocation[(int)reg] == true)
	{
	//	g_app->EmitError("Parser attempted to allocate more registers than those available, registers are being stored in stack.", ES_PARSER, EL_WARNING, _currentToken.Line, _currentToken.Column, _currentToken.Path);
		g_app->EmitError("Internal error, attempt to allocate register that was already allocated.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}
	if (val == false && _registerAllocation[(int)reg] == false)
	{
	//	g_app->EmitError("Parser attempted to allocate more registers than those available, registers are being stored in stack.", ES_PARSER, EL_WARNING, _currentToken.Line, _currentToken.Column, _currentToken.Path);
		g_app->EmitError("Internal error, attempt to deallocate register that was not allocated.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}

	_registerAllocation[(int)reg] = val;
	_registerAllocationDataType[(int)reg] = NULL;

	// TODO: Won't deallocate correct one;

	//if (_registerAllocation[(int)reg] > 1 && !(reg >= REG_ST0 && reg <= REG_ST3))
	//{
		// Push current value of register into stack.
	//	(new IntDataType())->Push(reg);
	//}
}

// --------------------------------------------------------------------------------
//	Deallocates a register that has been registered by part of the code.
// --------------------------------------------------------------------------------
void Parser::DeallocateRegister(Register reg)
{
	DataType* type = _registerAllocationDataType[(int)reg];
	
	//printf("\nDeallocating Register ...\n");

	if (type != NULL && (type->GetPrimitive() == PDT_LONG || type->GetPrimitive() == PDT_DOUBLE))
	{
		SetRegisterAllocation(Register64LoBitsTwin(reg), false);
		SetRegisterAllocation(Register64HiBitsTwin(reg), false);
		return;
	}
	//printf(("-Deallocated register "+StringHelper::IntToString(reg)+" to false\n").c_str());
			
	if (_registerAllocation[(int)reg] == 0)
	{
		g_app->EmitError("Internal error, attempt to deallocate register that was not allocated.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	}

	_registerAllocation[(int)reg] = 0;
	_registerAllocationDataType[(int)reg] = NULL;

	//if (_registerAllocation[(int)reg] > 0 && !(reg >= REG_ST0 && reg <= REG_ST3))
	//{
		// Pop current value into register.
	//	(new IntDataType())->Pop(reg);
	//}
}

// --------------------------------------------------------------------------------
//  This method will parse an expression. This is essentially just an alias for
//  ParseExpressionLevel1.
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpression()
{
	return ParseExpressionLevel1();
}

// --------------------------------------------------------------------------------
//  This method will parse the 1st level of an expression.
//  This layer deals with logical operators.
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel1()
{
	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	DataType* rValueDataType = new VoidDataType();

	// Parse the l-value
	lValueDataType = ParseExpressionLevel2();

	// Parse any following r-values.
	while (true)
	{
		// Check the next token is an r-value.
		Token operatorToken = LookAheadToken();
		if (!(operatorToken.ID == TOKEN_OP_LOGICAL_AND ||
			  operatorToken.ID == TOKEN_OP_LOGICAL_OR))
			break;
		NextToken();

		// Parse the R-Value
		rValueDataType = ParseExpressionLevel2();

		// Emit the instructions.
		if (_currentPass == 2)
		{
			DataType* finalType = new BoolDataType();

			// Data types not valid? Cast then :3
			if (rValueDataType->RequiresCast(finalType) ||
				lValueDataType->RequiresCast(finalType))
			{
				// TODO: URGENT Change both data types to boolean.

				// Allocate a register to store the values in.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType;
				Register rValueRegister = AllocateRegister(rValueDataType);
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType;
				Register lValueRegister = AllocateRegister(lValueDataType);
				lValueDataType->Pop(lValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				/*
				// Convert the l value that still on the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				rValueRegister = AllocateRegister(rValueDataType);
				lValueRegister = AllocateRegister(lValueDataType);
								
				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);
				*/

				// Set the l value data type to our implicit cast type.
				lValueDataType = finalType;
				rValueDataType = finalType;
			}

			//Register register1 = AllocateRegister(lValueDataType, 0);
			//Register register2 = AllocateRegister(rValueDataType, 1);

			// Pop the R-Value and L-Value into registers.
			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register2);

			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register1);

			// TODO: CASTING VALUES
			// TODO: Check Data Types

			// Perform operation.
			switch (operatorToken.ID)
			{
				case TOKEN_OP_LOGICAL_AND:
				{
					if (_currentPass == 2)
					{										
						//  pop rvalue
						//  pop lvalue		
						//  cmp lvalue, 0
						//  je false
						//  cmp rvalue, 0
						//  je false
						//  pushd 1
						//  jmp true
						// false:
						//  pushd 0
						// true:

						JumpTarget* falseJumpTarget = new JumpTarget(_currentToken);
						JumpTarget* trueJumpTarget = new JumpTarget(_currentToken);

						Register rValueRegister = AllocateRegister(rValueDataType);
						Register lValueRegister = AllocateRegister(lValueDataType);
					
						// Pop the r value off the stack.
						rValueDataType->Pop(rValueRegister);

						// Pop the l value off the stack.
						lValueDataType->Pop(lValueRegister);

						// Compare l-register with 0.
						instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
						instruction->AddRegisterOperand(lValueRegister);
						instruction->AddIntLiteralOperand(0);

						// If equal to 0, jump to false target.
						instruction = new Instruction(_currentScope, OPCODE_JE, _currentToken);
						instruction->AddLabelOperand(falseJumpTarget);

						// Compare r-register with 0.
						instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
						instruction->AddRegisterOperand(rValueRegister);
						instruction->AddIntLiteralOperand(0);

						// If equal to 0, jump to false target.
						instruction = new Instruction(_currentScope, OPCODE_JE, _currentToken);
						instruction->AddLabelOperand(falseJumpTarget);

						// Push true and jump to end.
						instruction = new Instruction(_currentScope, OPCODE_PUSHD, _currentToken);
						instruction->AddIntLiteralOperand(1);
						
						// Jump tro true target.
						instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
						instruction->AddLabelOperand(trueJumpTarget);

						// Bind the false jump target.
						falseJumpTarget->Bind(_currentScope);

						// Push false and jump to end.
						instruction = new Instruction(_currentScope, OPCODE_PUSHD, _currentToken);
						instruction->AddIntLiteralOperand(0);
						
						// Bind the true jump target.
						trueJumpTarget->Bind(_currentScope);

						// Deallocate the register.
						DeallocateRegister(lValueRegister);
						DeallocateRegister(rValueRegister);
					}

					break;
				}
				case TOKEN_OP_LOGICAL_OR:
				{
					if (_currentPass == 2)
					{				

						//  pop rvalue
						//  pop lvalue
						//  cmp lvalue, 0
						//  jne true
						//  cmp rvalue, 0
						//  jne true
						//  pushd 0
						//  jmp false
						// true:
						//  pushd 1
						// true:

						JumpTarget* falseJumpTarget = new JumpTarget(_currentToken);
						JumpTarget* trueJumpTarget = new JumpTarget(_currentToken);

						Register rValueRegister = AllocateRegister(rValueDataType);
						Register lValueRegister = AllocateRegister(lValueDataType);
											
						// Pop the r value off the stack.
						rValueDataType->Pop(rValueRegister);

						// Pop the l value off the stack.
						lValueDataType->Pop(lValueRegister);

						// Compare l-register with 0.
						instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
						instruction->AddRegisterOperand(lValueRegister);
						instruction->AddIntLiteralOperand(0);

						// If not equal to 0, jump to true target.
						instruction = new Instruction(_currentScope, OPCODE_JNE, _currentToken);
						instruction->AddLabelOperand(trueJumpTarget);

						// Compare r-register with 0.
						instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
						instruction->AddRegisterOperand(rValueRegister);
						instruction->AddIntLiteralOperand(0);
						
						// If not equal to 0, jump to true target.
						instruction = new Instruction(_currentScope, OPCODE_JNE, _currentToken);
						instruction->AddLabelOperand(trueJumpTarget);

						// Push false and jump to end.
						instruction = new Instruction(_currentScope, OPCODE_PUSHD, _currentToken);
						instruction->AddIntLiteralOperand(0);
						
						// Jump to false target.
						instruction = new Instruction(_currentScope, OPCODE_JMP, _currentToken);
						instruction->AddLabelOperand(falseJumpTarget);

						// Bind the true jump target.
						trueJumpTarget->Bind(_currentScope);

						// Push true and jump to end.
						instruction = new Instruction(_currentScope, OPCODE_PUSHD, _currentToken);
						instruction->AddIntLiteralOperand(1);
						
						// Bind the false jump target.
						falseJumpTarget->Bind(_currentScope);

						// Deallocate the register.
						DeallocateRegister(lValueRegister);
						DeallocateRegister(rValueRegister);
					}

					break;
				}
			}

			// Push result onto stack.
			//instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
			//instruction->AddRegisterOperand(register1);
		}
	}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This method will parse the 2nd level of an expression.
//  This layer deals with conditions operators. 
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel2()
{
//	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	DataType* rValueDataType = new VoidDataType();

	// Parse the l-value
	lValueDataType = ParseExpressionLevel3();

	// Parse any following r-values.
	while (true)
	{
		// Check the next token is an r-value.
		Token operatorToken = LookAheadToken();
		if (!(operatorToken.ID == TOKEN_OP_LESS ||
			  operatorToken.ID == TOKEN_OP_GREATER ||
			  operatorToken.ID == TOKEN_OP_LESS_EQUAL ||
			  operatorToken.ID == TOKEN_OP_GREATER_EQUAL ||
			  operatorToken.ID == TOKEN_OP_NOT_EQUAL ||
			  operatorToken.ID == TOKEN_OP_EQUAL))
			break;
		NextToken();

		// Parse the R-Value
		rValueDataType = ParseExpressionLevel3();

		// Emit the instructions.
		if (_currentPass == 2)
		{
			// Data types not valid? Cast then :3
			if (rValueDataType->RequiresCast(lValueDataType))
			{
				// Work out which data type to cast everything to.
				DataType* finalType = (lValueDataType->GetImplicityCastPriority() > 
									   rValueDataType->GetImplicityCastPriority() 
									   ? lValueDataType : rValueDataType);
				
				// Allocate a register to store the values in.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType;
				Register rValueRegister = AllocateRegister(rValueDataType);
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType;
				Register lValueRegister = AllocateRegister(lValueDataType);
				lValueDataType->Pop(lValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				/*
				// Convert the r-value that is now on the stack.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				Register rValueRegister = AllocateRegister(rValueDataType);
				Register lValueRegister = AllocateRegister(lValueDataType);
				
				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Convert the l value that still on the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				rValueRegister = AllocateRegister(rValueDataType);
				lValueRegister = AllocateRegister(lValueDataType);
								
				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Set the l value data type to our implicit cast type.
				lValueDataType = finalType;*/
			}

			//Register register1 = AllocateRegister(lValueDataType, 0);
			//Register register2 = AllocateRegister(rValueDataType, 1);

			// Pop the R-Value and L-Value into registers.
			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register2);

			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register1);
		
			// TODO: CASTING VALUES
			// TODO: Check Data Types
			
			// Compare registers.
			//instruction = new Instruction(_currentScope, OPCODE_CMP, _currentToken);
			//instruction->AddRegisterOperand(register1);
			//instruction->AddRegisterOperand(register2);
					
			// Perform operation.
			switch (operatorToken.ID)
			{
				case TOKEN_OP_LESS:
				{
					lValueDataType = lValueDataType->OpCompareLess();
				//	instruction = new Instruction(_currentScope, OPCODE_SETL, _currentToken);
				//	instruction->AddRegisterOperand(register1);
					break;
				}
				case TOKEN_OP_GREATER:
				{
					lValueDataType = lValueDataType->OpCompareGreater();
				//	instruction = new Instruction(_currentScope, OPCODE_SETG, _currentToken);
				//	instruction->AddRegisterOperand(register1);
					break;
				}
				case TOKEN_OP_LESS_EQUAL:
				{
					lValueDataType = lValueDataType->OpCompareLessEqual();
				//	instruction = new Instruction(_currentScope, OPCODE_SETLE, _currentToken);
				//	instruction->AddRegisterOperand(register1);
					break;
				}
				case TOKEN_OP_GREATER_EQUAL:
				{
					lValueDataType = lValueDataType->OpCompareGreaterEqual();
				//	instruction = new Instruction(_currentScope, OPCODE_SETGE, _currentToken);
				//	instruction->AddRegisterOperand(register1);
					break;
				}
				case TOKEN_OP_EQUAL:
				{
					lValueDataType = lValueDataType->OpCompareEqual();
				//	instruction = new Instruction(_currentScope, OPCODE_SETE, _currentToken);
				//	instruction->AddRegisterOperand(register1);
					break;
				}
				case TOKEN_OP_NOT_EQUAL:
				{
					lValueDataType = lValueDataType->OpCompareNotEqual();
				//	instruction = new Instruction(_currentScope, OPCODE_SETNE, _currentToken);
				//	instruction->AddRegisterOperand(register1);
					break;
				}
			}

			// Push result onto stack.
		//	instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
		//	instruction->AddRegisterOperand(register1);
		}
	}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This method will parse the 3rd level of an expression.
//  This layer deals with bitwise operators (And, Or and XOr, .etc).
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel3()
{
//	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	DataType* rValueDataType = new VoidDataType();

	// Parse the l-value
	lValueDataType = ParseExpressionLevel4();

	// Parse any following r-values.
	while (true)
	{
		// Check the next token is an r-value.
		Token operatorToken = LookAheadToken();
		if (!(operatorToken.ID == TOKEN_OP_BITWISE_SHL ||
			  operatorToken.ID == TOKEN_OP_BITWISE_SHR ||
			  operatorToken.ID == TOKEN_OP_BITWISE_AND ||
			  operatorToken.ID == TOKEN_OP_BITWISE_OR ||
			  operatorToken.ID == TOKEN_OP_BITWISE_XOR ||
			  operatorToken.ID == TOKEN_OP_MODULUS))
			break;
		NextToken();

		// Parse the R-Value
		rValueDataType = ParseExpressionLevel4();

		// Emit the instructions.
		if (_currentPass == 2)
		{
			// Data types not valid? Cast then :3
			if (rValueDataType->RequiresCast(lValueDataType))
			{
				// Work out which data type to cast everything to.
				DataType* finalType = (lValueDataType->GetImplicityCastPriority() > 
									   rValueDataType->GetImplicityCastPriority() 
									   ? lValueDataType : rValueDataType);

				// Allocate a register to store the values in.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType;
				Register rValueRegister = AllocateRegister(rValueDataType);
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType;
				Register lValueRegister = AllocateRegister(lValueDataType);
				lValueDataType->Pop(lValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				/*
				// Convert the r-value that is now on the stack.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				Register rValueRegister = AllocateRegister(rValueDataType);
				Register lValueRegister = AllocateRegister(lValueDataType);
				
				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Convert the l value that still on the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				rValueRegister = AllocateRegister(rValueDataType);
				lValueRegister = AllocateRegister(lValueDataType);
								
				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Set the l value data type to our implicit cast type.
				lValueDataType = finalType;
				*/
			}

			//Register register1 = AllocateRegister(lValueDataType, 0);
			//Register register2 = AllocateRegister(rValueDataType, 1);

			// Pop the R-Value and L-Value into registers.
			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register2);

			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register1);
		
			// TODO: CASTING VALUES
			// TODO: Check Data Types

			// Perform operation.
			switch (operatorToken.ID)
			{
				case TOKEN_OP_BITWISE_SHL:
				{
					lValueDataType = lValueDataType->OpBitwiseSHL();
					//instruction = new Instruction(_currentScope, OPCODE_SHL, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
				case TOKEN_OP_BITWISE_SHR:
				{
					lValueDataType = lValueDataType->OpBitwiseSHR();
					//instruction = new Instruction(_currentScope, OPCODE_SHR, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
				case TOKEN_OP_BITWISE_AND:
				{
					lValueDataType = lValueDataType->OpBitwiseAND();
					//instruction = new Instruction(_currentScope, OPCODE_AND, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
				case TOKEN_OP_BITWISE_OR:
				{
					lValueDataType = lValueDataType->OpBitwiseOR();
					//instruction = new Instruction(_currentScope, OPCODE_OR, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
				case TOKEN_OP_BITWISE_XOR:
				{
					lValueDataType = lValueDataType->OpBitwiseXOR();
					//instruction = new Instruction(_currentScope, OPCODE_XOR, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
				case TOKEN_OP_MODULUS:
				{
					lValueDataType = lValueDataType->OpBitwiseMOD();
					/*
					instruction = new Instruction(_currentScope, OPCODE_IDIV, _currentToken);
					instruction->AddRegisterOperand(register1);
					instruction->AddRegisterOperand(register2);

					// Change register 1 to the remainder register for the DIV instruction (modulus).
					register1 = REG_DX;
					*/

//					TODO
					break;
				}
			}

			// Push result onto stack.
			//instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
			//instruction->AddRegisterOperand(register1);
		}
	}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This method will parse the 4th level of an expression.
//  This layer deals with addition and subtraction operators.
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel4()
{
//	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	DataType* rValueDataType = new VoidDataType();

	// Parse the l-value
	lValueDataType = ParseExpressionLevel5();

	// Parse any following r-values.
	while (true)
	{
		// Check the next token is an r-value.
		Token operatorToken = LookAheadToken();
		if (!(operatorToken.ID == TOKEN_OP_ADD ||
			  operatorToken.ID == TOKEN_OP_SUBTRACT))
			break;
		NextToken();

		// Parse the R-Value
		rValueDataType = ParseExpressionLevel5();

		// Emit the instructions.
		if (_currentPass == 2)
		{
			// Data types not valid? Cast then :3
			if (rValueDataType->RequiresCast(lValueDataType))
			{
				// Work out which data type to cast everything to.
				DataType* finalType = (lValueDataType->GetImplicityCastPriority() > 
									   rValueDataType->GetImplicityCastPriority() 
									   ? lValueDataType : rValueDataType);
			
				// Allocate a register to store the values in.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType;
				Register rValueRegister = AllocateRegister(rValueDataType);
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType;
				Register lValueRegister = AllocateRegister(lValueDataType);
				lValueDataType->Pop(lValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Convert the r-value that is now on the stack.
				//ImplicitTypeCast(lValueDataType, rValueDataType)
				//rValueDataType->CastTo(finalType);
				//rValueDataType = finalType; // Fix - Problem here, still trying to allocate variable with old datatype.
											//       Even after its been cast. Things go tits up :3. 
				/*
				// Allocate a register to store the values in.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType;
				Register rValueRegister = AllocateRegister(rValueDataType);
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType;
				Register lValueRegister = AllocateRegister(lValueDataType);
				rValueDataType->Pop(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Convert the l value that still on the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				rValueRegister = AllocateRegister(rValueDataType);
				lValueRegister = AllocateRegister(lValueDataType);
								
				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Set the l value data type to our implicit cast type.
				lValueDataType = finalType;*/
			}

			//Register register1 = AllocateRegister(lValueDataType, 0);
			//Register register2 = AllocateRegister(rValueDataType, 1);

			// Pop the R-Value and L-Value into registers.
			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register2);

			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register1);
		
			// TODO: CASTING VALUES
			// TODO: Check Data Types

			// Perform operation.
			switch (operatorToken.ID)
			{
				case TOKEN_OP_ADD:
				{
					lValueDataType = lValueDataType->OpAdd();
					//instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
				case TOKEN_OP_SUBTRACT:
				{
					lValueDataType = lValueDataType->OpSubtract();
					//instruction = new Instruction(_currentScope, OPCODE_SUB, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
			}

			// Push result onto stack.
			//instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
			//instruction->AddRegisterOperand(register1);
		}
	}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This method will parse the 5th level of an expression.
//  This layer deals with multiply and divide operators.
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel5()
{
//	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	DataType* rValueDataType = new VoidDataType();

	// Parse the l-value
	lValueDataType = ParseExpressionLevel6();

	// Parse any following r-values.
	while (true)
	{
		// Check the next token is an r-value.
		Token operatorToken = LookAheadToken();
		if (!(operatorToken.ID == TOKEN_OP_MULTIPLY ||
			  operatorToken.ID == TOKEN_OP_DIVIDE))
			break;
		NextToken();

		// Parse the R-Value
		rValueDataType = ParseExpressionLevel6();

		// Emit the instructions.
		if (_currentPass == 2)
		{
			// Data types not valid? Cast then :3
			if (rValueDataType->RequiresCast(lValueDataType))
			{
				// Work out which data type to cast everything to.
				DataType* finalType = (lValueDataType->GetImplicityCastPriority() > 
									   rValueDataType->GetImplicityCastPriority() 
									   ? lValueDataType : rValueDataType);
				
				// Allocate a register to store the values in.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType;
				Register rValueRegister = AllocateRegister(rValueDataType);
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType;
				Register lValueRegister = AllocateRegister(lValueDataType);
				lValueDataType->Pop(lValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				/*
				// Convert the r-value that is now on the stack.
				rValueDataType->CastTo(finalType);
				rValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				Register rValueRegister = AllocateRegister(rValueDataType);
				Register lValueRegister = AllocateRegister(lValueDataType);
				
				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Convert the l value that still on the stack.
				lValueDataType->CastTo(finalType);
				lValueDataType = finalType; // Fix

				// Allocate a register to store the values in.
				rValueRegister = AllocateRegister(rValueDataType);
				lValueRegister = AllocateRegister(lValueDataType);
								
				// Pop the l value off the stack.
				rValueDataType->Pop(lValueRegister);

				// Pop the r value off the stack.
				rValueDataType->Pop(rValueRegister);

				// Push the l value on the stack.
				rValueDataType->Push(lValueRegister);

				// Push the r value on the stack.
				rValueDataType->Push(rValueRegister);
				
				// Deallocate the register.
				DeallocateRegister(lValueRegister);
				DeallocateRegister(rValueRegister);

				// Set the l value data type to our implicit cast type.
				lValueDataType = finalType;
				*/
			}

			//Register register1 = AllocateRegister(lValueDataType, 1); // Offset by 1, we don't want to get EAX because idiv works on it.
			//Register register2 = AllocateRegister(rValueDataType, 2);

			// Pop the R-Value and L-Value into registers.
			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register2);

			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register1);
		
			// TODO: CASTING VALUES
			// TODO: Check Data Types

			// Perform operation.
			switch (operatorToken.ID)
			{
				case TOKEN_OP_MULTIPLY:
				{
					lValueDataType = lValueDataType->OpMultiply();
					//instruction = new Instruction(_currentScope, OPCODE_IMUL, _currentToken);
					//instruction->AddRegisterOperand(register1);
					//instruction->AddRegisterOperand(register2);
					break;
				}
				case TOKEN_OP_DIVIDE:
				{
					lValueDataType = lValueDataType->OpDivide();
					// MOV Dividend into EAX
					// CDQ
					// IDIV Divisor
					// EAX=Quotient
					//EDX=Remainder
					
					/*
					instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
					instruction->AddRegisterOperand(REG_EDX);
					instruction->AddIntLiteralOperand(0);

					instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
					instruction->AddRegisterOperand(REG_EAX);
					instruction->AddRegisterOperand(register1);

					instruction = new Instruction(_currentScope, OPCODE_CDQ, _currentToken);

					instruction = new Instruction(_currentScope, OPCODE_IDIV, _currentToken);
					instruction->AddRegisterOperand(register2);

					instruction = new Instruction(_currentScope, OPCODE_MOV, _currentToken);
					instruction->AddRegisterOperand(register1);
					instruction->AddRegisterOperand(REG_EAX);
					*/

					break;
				}
			}

			// Push result onto stack.
			//instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
			//instruction->AddRegisterOperand(register1);
		}
	}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This method will parse the 6th level of an expression.
//  This layer deals with unary operators (negatate, posatate, bitwise complement, boolean not, .etc)
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel6()
{	
//	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	DataType* rValueDataType = new VoidDataType();
	bool castExists = false;
	DataType* castDataType = new VoidDataType();
	bool unaryExists = false;

	// Is there a pending cast on the left side of the unary op?
	int startTokenIndex = _tokenIndex;
	Token token = LookAheadToken(1);
	if (LookAheadToken(1).ID == TOKEN_OP_LESS &&
		LookAheadToken(3).ID == TOKEN_OP_GREATER)
	{
		ExpectToken(TOKEN_OP_LESS);
		castDataType = ParseDataType(true);
		if (castDataType == NULL)
		{
			ExpectToken(TOKEN_OP_GREATER);
			//_tokenIndex = startTokenIndex - 1;
			//NextToken();
		}
		else
		{
			castExists = true;
			ExpectToken(TOKEN_OP_GREATER);
		}
	}

	// Is there a unary operator next?
	Token operatorToken = LookAheadToken();
	if (operatorToken.ID == TOKEN_OP_SUBTRACT ||
		operatorToken.ID == TOKEN_OP_BITWISE_NOT || 
		operatorToken.ID == TOKEN_OP_LOGICAL_NOT)
	{
		NextToken();
		unaryExists = true;
	}

	// Parse the L-Value.
	lValueDataType = ParseExpressionLevel7();

	// Perform unary operation?
	if (unaryExists == true)
	{
		if (_currentPass == 2)
		{
			//Register register1 = AllocateRegister(lValueDataType, 0);

			// Pop the L-Value into registers.
			//instruction = new Instruction(_currentScope, OPCODE_POP, _currentToken);
			//instruction->AddRegisterOperand(register1);

			// Perform operation.
			switch (operatorToken.ID)
			{
				case TOKEN_OP_SUBTRACT:
					{
						lValueDataType = lValueDataType->OpUnaryNeg();
						//instruction = new Instruction(_currentScope, OPCODE_NEG, _currentToken);
						//instruction->AddRegisterOperand(register1);
						break;
					}
				case TOKEN_OP_BITWISE_NOT:
					{
						lValueDataType = lValueDataType->OpUnaryNot();
						//instruction = new Instruction(_currentScope, OPCODE_NOT, _currentToken);
						//instruction->AddRegisterOperand(register1);
						break;
					}
				case TOKEN_OP_LOGICAL_NOT:
					{
						lValueDataType = lValueDataType->OpUnaryLogicalNot();
						// TODO, if op is 0, return 1, else 0
						//instruction = new Instruction(_currentScope, OPCODE_NOT, _currentToken);
						//instruction->AddRegisterOperand(register1);
						break;
					}
			}

			// Push result onto stack.
			//instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
			//instruction->AddRegisterOperand(register1);
		}
	}

	// Perform casting operation.
	if (castExists == true)
	{
		if (_currentPass == 2)
			lValueDataType->CastTo(castDataType, true);
		lValueDataType = castDataType;
	}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This method will parse the 7th level of an expression.
//  This layer deals with member accessing, indexing and function calls. It also
//  deals with assignment operators (=, +=, -=, .etc).
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel7()
{
//	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	DataType* rValueDataType = new VoidDataType();

	// Parse the l-value
	lValueDataType = ParseExpressionLevel8();

	// Parse any following r-values.
	while (true)
	{
		// Check the next token is an r-value.
		Token operatorToken = LookAheadToken();
		if (!(operatorToken.ID == TOKEN_CHAR_PERIOD))
			break;
		NextToken();

		// Parse depending on the operator.
		switch (operatorToken.ID)
		{
			// If its a period then lets find the variable, and push its value
			// into the stack or assign it.
			case TOKEN_CHAR_PERIOD:
			{
				// Check that the left data type is a class reference.
				if (_currentPass == 2 && lValueDataType->GetPrimitive() != PDT_CLASS)
					g_app->EmitError("Indirection operator (.) can only be used on class references.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

				// Read in the identifier of the member variable.
				std::string identifier = ExpectToken(TOKEN_LITERAL_IDENTIFIER).Literal;

				// Try and find the symbol for the member variable.
				ClassSymbol* classSymbol = ((ClassDataType*)lValueDataType)->Class;
				Symbol* memberSymbol = NULL;

				// Get the symbol.
				if (_currentPass == 2)
				{
					memberSymbol = classSymbol->FindChild(_currentToken.Literal, false, false);
					if (_currentPass == 2 && memberSymbol == NULL)
						g_app->EmitError("Undefined member \"" + _currentToken.Literal + "\" of class \"" + classSymbol->Identifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				}

				// Are we performing an assignment?
				if (LookAheadToken().ID == TOKEN_OP_ASSIGN ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_ADD ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_SUBTRACT ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_MULTIPLY ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_DIVIDE ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_MODULUS ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_AND ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_OR ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_XOR ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_NOT ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_SHL ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_SHR)
				{
					if (_currentPass == 2 && memberSymbol->GetSymbolType() != ST_VARIABLE)
						g_app->EmitError("Found identifier, but symbol was of the wrong type \"" + _currentToken.Literal + "\", expecting variable.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
					
					VariableSymbol* variable = (VariableSymbol*)memberSymbol;
					
					if (_currentPass == 2 && variable->AccessModifier != AM_PUBLIC)
						g_app->EmitError("Unable to access \"" + variable->Identifier + "\", variable is not publicly accessible.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);	

					ParseAssignment(variable);
			
					if (_currentPass == 2)
					{
						lValueDataType = variable->DataType;

						variable->DataType->PushVariableValue(variable); 

						// Remove the class reference from the stack, the variables value will contain that.
						Register reg1 = AllocateRegister(variable->DataType);

						variable->DataType->Pop(reg1);
						lValueDataType->PopDestroy();
						variable->DataType->Push(reg1);

						DeallocateRegister(reg1);
					}
				}

				// If not just push the value of the variable. 
				else
				{
					// Are we doing a function call?
					if (LookAheadToken().ID == TOKEN_CHAR_OPEN_PARENTHESIS)
					{
						if (_currentPass == 2 && memberSymbol->GetSymbolType() != ST_FUNCTION)
							g_app->EmitError("Found identifier, but symbol was of the wrong type \"" + _currentToken.Literal + "\", expecting function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
					
						// Calculate number of paramters.
						int count = CalcFunctionCallParameters();
						// TODO: add check here for method/non-method ambiguity test.

						FunctionSymbol* method = NULL;
						if (_currentPass == 2)
						{
							method = classSymbol->FindChildFunction(memberSymbol->Identifier, count, false, false);//(FunctionSymbol*)memberSymbol;
							if (method == NULL)
								method = classSymbol->FindChildFunction(memberSymbol->Identifier, count + 1, false, false);
							if (method == NULL)
								g_app->EmitError("No function found with name \"" + _currentToken.Literal + "\" taking "+StringHelper::IntToString(count)+" parameters.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
						}

						if (_currentPass == 2 && method->AccessModifier != AM_PUBLIC)
							g_app->EmitError("Unable to access \"" + method->Identifier + "\", method is not publicly accessible.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);	

						// If its not a method but a normal function, then pop out the 
						// "this" reference in the stack.
						if (_currentPass == 2  && method->IsMethod == false)
							lValueDataType->PopDestroy();

						// Invoke the function, the ParseFunctionCall method will deal
						// with method/function formalities.
						DataType* returnDT;
						if (_currentPass == 2)
							returnDT = ParseFunctionCall(method, ((ClassDataType*)lValueDataType)->DoNotUseVirutal, method->Identifier, method->IsMethod);
						else
							returnDT = ParseFunctionCall();

						// Pop out the "this" reference in the stack if its a method.
						//if (method->IsMethod == true && _currentPass == 2)
						//	lValueDataType->PopDestroy();

						lValueDataType = returnDT;

						// Push return register onto the stack.
						if (_currentPass == 2)
						{
							if (lValueDataType->GetPrimitive() != PDT_VOID)
							{
								lValueDataType->PushReturn();
							}
						}
					}

					// Nope, just push the value of the member variable then.
					else
					{
						if (_currentPass == 2 && memberSymbol->GetSymbolType() != ST_VARIABLE)
							g_app->EmitError("Found identifier, but symbol was of the wrong type \"" + _currentToken.Literal + "\", expecting variable.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
						
						VariableSymbol* variable = (VariableSymbol*)memberSymbol;
								
						if (_currentPass == 2 && variable->AccessModifier != AM_PUBLIC)
							g_app->EmitError("Unable to access \"" + variable->Identifier + "\", variable is not publicly accessible.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);	
	
						if (_currentPass == 2)
						{
							lValueDataType = variable->DataType;

							variable->DataType->PushVariableValue(variable); 
							
							// Remove the class reference from the stack, the variables value will contain that.
							Register reg1 = AllocateRegister(variable->DataType);

							variable->DataType->Pop(reg1);
							lValueDataType->PopDestroy();
							variable->DataType->Push(reg1);

							DeallocateRegister(reg1);
						}
					}
				}


			}
		}
	}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This method will parse the 8th level of an expression.
//  This layer deals with constant operators.
// --------------------------------------------------------------------------------
DataType* Parser::ParseExpressionLevel8()
{
	Instruction* instruction;
	DataType* lValueDataType = new VoidDataType();
	//bool castExists = false;
	//DataType* castDataType = new VoidDataType();
	//bool unaryExists = false;

	// Is there a pending cast on the left side of this factor.
	//if (LookAheadToken(1).ID == TOKEN_CHAR_OPEN_PARENTHESIS &&
	//	LookAheadToken(3).ID == TOKEN_CHAR_CLOSE_PARENTHESIS)
	//{
	//	ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
	//	castDataType = ParseDataType();
	//	castExists = true;
	//	ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
	//}

	// Parse the next factor.
	switch (NextToken().ID)
	{
		// Gets the value of a register.
		case TOKEN_KEYWORD_REGISTER:
		{
			// Next read in the next token's identifier for the register name.
			ExpectToken(TOKEN_CHAR_OPEN_BRACKET);
			std::string regName = StringHelper::ToLower(NextToken().Literal);
			
			// Go through register and check its valid.
			bool found = false;
			for (int i = 0; i < REGISTER_COUNT; i++)
			{
				if (regName == RegisterMnemonics[i])
				{
					switch (RegisterSizes[i])
					{
					case 1:
						lValueDataType = new IntDataType();
						break;
					case 2:
						lValueDataType = new IntDataType();
						break;
					case 4:
						lValueDataType = new IntDataType();
						break;
					case 8:
						break;
					default:
						g_app->EmitError("Register \"" + regName + "\" has no comparable data type to return as.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
						break;
					}
					
					if (_currentPass == 2) 
						lValueDataType->Push((Register)i);

					found = true;
				}
			}

			ExpectToken(TOKEN_CHAR_CLOSE_BRACKET);

			if (found == false)
				g_app->EmitError("Undefined register \"" + regName + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
		
			break;
		}

		// Access super class method.
		case TOKEN_KEYWORD_SUPER:
		{
			// Get the method.
			FunctionSymbol* methodSymbol = dynamic_cast<FunctionSymbol*>(_currentScope);
			if (_currentPass == 2 && (methodSymbol == NULL || methodSymbol->IsMethod == false))
				g_app->EmitError("Super keyword can only be used within class methods.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

			// Get the class.
			ClassSymbol* classSymbol = dynamic_cast<ClassSymbol*>(methodSymbol->Scope);
			if (_currentPass == 2 && classSymbol == NULL)
				g_app->EmitError("Super keyword can only be used within class methods.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
			if (_currentPass == 2 && classSymbol->SuperClass == NULL)
				g_app->EmitError("Super keyword can only be used within derived classes.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

			if (_currentPass == 2)
			{
				// Find the "this" variable.
				VariableSymbol* thisVariable = (VariableSymbol*)methodSymbol->Children.at(methodSymbol->ParameterCount - 1);
				if (_currentPass == 2)
					thisVariable->DataType->PushVariableValue(thisVariable);
				
				// Return the data type.
				lValueDataType = new ClassDataType(classSymbol->SuperClass);
				((ClassDataType*)lValueDataType)->DoNotUseVirutal = true;
			}

			break;
		}

		// Variables and so forth.
		case TOKEN_LITERAL_IDENTIFIER:
		{	
			// Get the symbol.
			Symbol* namespaceScope = ParseNamespaceScope();

			std::string ident = _currentToken.Literal;
			Symbol* symbol = namespaceScope->FindChild(ident);
			if (_currentPass == 2 && symbol == NULL)
				g_app->EmitError("Undefined identifier \"" + _currentToken.Literal + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

			// Function call?
			if (LookAheadToken().ID == TOKEN_CHAR_OPEN_PARENTHESIS)
			{
				if (_currentPass == 2 && symbol->GetSymbolType() != ST_FUNCTION)
					g_app->EmitError("Found identifier, but symbol was of the wrong type \"" + _currentToken.Literal + "\", expecting function.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
							
				// Calculate paramter count.
				int paramCount = CalcFunctionCallParameters();
				symbol = namespaceScope->FindChildFunction(ident, paramCount);
				if (symbol == NULL)
					symbol = namespaceScope->FindChildFunction(ident, paramCount + 1);
				if (_currentPass == 2 && symbol == NULL)
					g_app->EmitError("No function named \"" + _currentToken.Literal + "\" that takes "+StringHelper::IntToString(paramCount)+" arguments.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

				FunctionSymbol* function = symbol == NULL ? NULL : (FunctionSymbol*)symbol;
				FunctionSymbol* functionScope = NULL;
				VariableSymbol* thisVariable = NULL;
				if (_currentPass == 2 && function->IsMethod == true)
				{
					// If this is a method, then we need to push the "this" reference variable.
					if (_currentScope->GetSymbolType() != ST_FUNCTION ||
						((FunctionSymbol*)_currentScope)->IsMethod == false)
						g_app->EmitError("Implicit method calls can only be invoked from within methods.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
					
					functionScope = (FunctionSymbol*)_currentScope;

					// Find the "this" variable.
					thisVariable = (VariableSymbol*)functionScope->Children.at(functionScope->ParameterCount - 1);
					if (_currentPass == 2)
						thisVariable->DataType->PushVariableValue(thisVariable);
				}
				
				lValueDataType = ParseFunctionCall(function);

				// Pop off the "this" variable.
				//if (_currentPass == 2 && function->IsMethod == true)
				//	thisVariable->DataType->PopDestroy();

				// Push return register onto the stack.
				if (_currentPass == 2)
					if (lValueDataType->GetPrimitive() != PDT_VOID)
						lValueDataType->PushReturn();
			}

			// Variable?
			else
			{
				if (_currentPass == 2 && symbol->GetSymbolType() != ST_VARIABLE)
					g_app->EmitError("Found identifier, but symbol was of the wrong type \"" + _currentToken.Literal + "\", expecting variable.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
								
				VariableSymbol* variable = symbol == NULL ? NULL : (VariableSymbol*)symbol;
				
				FunctionSymbol* functionScope = NULL;
				VariableSymbol* thisVariable = NULL;
				if (_currentPass == 2 && variable->VariableType == VT_FIELD)
				{
					// If this is a method, then we need to push the "this" reference variable.
					if (_currentScope->GetSymbolType() != ST_FUNCTION ||
						((FunctionSymbol*)_currentScope)->IsMethod == false)
						g_app->EmitError("Implicit class field access can only be invoked from within methods.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
					
					functionScope = (FunctionSymbol*)_currentScope;

					// Find the "this" variable.
					thisVariable = (VariableSymbol*)functionScope->Children.at(functionScope->ParameterCount - 1);
					if (_currentPass == 2)
						thisVariable->DataType->PushVariableValue(thisVariable);
				}

				// Are we performing an assignment?
				if (LookAheadToken().ID == TOKEN_OP_ASSIGN ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_ADD ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_SUBTRACT ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_MULTIPLY ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_DIVIDE ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_MODULUS ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_AND ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_OR ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_XOR ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_NOT ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_SHL ||
					LookAheadToken().ID == TOKEN_OP_ASSIGN_BITWISE_SHR)
				{
					ParseAssignment(variable);
				}

				if (variable != NULL)
					lValueDataType = variable->DataType;

				if (_currentPass == 2)
				{
					variable->DataType->PushVariableValue(variable);
				}
				
				// Pop off the "this" variable.
				if (_currentPass == 2 && variable->VariableType == VT_FIELD)
				{
					Register reg1 = AllocateRegister(variable->DataType);

					variable->DataType->Pop(reg1);
					thisVariable->DataType->PopDestroy();
					variable->DataType->Push(reg1);

					DeallocateRegister(reg1);
				}
			}

			break;
		}
		case TOKEN_LITERAL_STRING:
		{
			lValueDataType = new StringDataType();
			
			if (_currentPass == 2)
				lValueDataType->PushLiteral(_currentToken.Literal);

			break;
		}
		case TOKEN_LITERAL_FLOAT:
		{
			lValueDataType = new FloatDataType();
			
			if (_currentPass == 2)
				lValueDataType->PushLiteral(_currentToken.Literal);

			break;
		}
		case TOKEN_LITERAL_DOUBLE:
		{
			lValueDataType = new DoubleDataType();
			
			if (_currentPass == 2)
				lValueDataType->PushLiteral(_currentToken.Literal);

			break;
		}
		case TOKEN_LITERAL_INTEGER:
		{
			//if (_currentPass == 2)
			//{
			//	instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
			//	instruction->AddStringLiteralOperand(_currentToken.Literal);
			//}
			
			signed long long val = StringHelper::StringToLong(_currentToken.Literal);
			signed long long minBound = -2147483647;
			signed long long maxBound = 2147483646;

			if (val < minBound || val > maxBound)
			{
				lValueDataType = new LongDataType();
				if (_currentPass == 2)
					lValueDataType->PushLiteral(_currentToken.Literal);
			}
			else
			{
				lValueDataType = new IntDataType();
				if (_currentPass == 2)
					lValueDataType->PushLiteral(_currentToken.Literal);
			}

			break;
		}
		//case TOKEN_KEYWORD_THIS:
		//{
			// TODO
		//	break;
		//}
		case TOKEN_KEYWORD_NEW:
		{
			//CalcFunctionCallParameters()
			std::string identifier = ExpectToken(TOKEN_LITERAL_IDENTIFIER).Literal;
		
			// Find class symbol.
			Symbol* namespaceSymbol = ParseNamespaceScope();

			identifier = _currentToken.Literal;
			Symbol* symbol = namespaceSymbol->FindChild(identifier);
			ClassSymbol* classSymbol = NULL;

			if (_currentPass == 2)
			{
				if (symbol == NULL)
					g_app->EmitError("Undefined identifier \"" + identifier + "\".", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				if (symbol->GetSymbolType() != ST_CLASS)
					g_app->EmitError("Found identifier \"" + identifier + "\", but it was not of correct type (expecting class).", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

				classSymbol = (ClassSymbol*)symbol;
				lValueDataType = new ClassDataType(classSymbol);

				// Push the class definition index.
				instruction = new Instruction(_currentScope, OPCODE_PUSH, _currentToken);
				instruction->AddIntLiteralOperand(classSymbol->ClassDefinitionIndex);

				// Invoke class allocation function.
				instruction = new Instruction(_currentScope, OPCODE_CALL, _currentToken);
				instruction->AddStringLiteralOperand("_icr__AllocateObject");
				
				// Cleanup Stack
				instruction = new Instruction(_currentScope, OPCODE_ADD, _currentToken);
				instruction->AddRegisterOperand(REG_ESP);
				instruction->AddIntLiteralOperand(4);

				// Push pointer to new class memory into stack.
				lValueDataType->PushReturn();
			}

			// Parse the parameterized function call.
//			if (LookAheadToken(2).ID != TOKEN_CHAR_CLOSE_PARENTHESIS)
//			{
				// Get new symbol.
				FunctionSymbol* newSymbol = NULL;
				if (_currentPass == 2)
				{
					// Work out the number of parameters.				
					int parameterCount = CalcFunctionCallParameters() + 1;
	
					newSymbol = (FunctionSymbol*)symbol->FindChildFunction("new", parameterCount, false, false);
					if (newSymbol == NULL)
						newSymbol = (FunctionSymbol*)symbol->FindChildFunction("new", parameterCount + 1, false, false);
					if (newSymbol == NULL)
						g_app->EmitError("No constructor method defined with "+StringHelper::IntToString(parameterCount)+" parameters.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
				
					// Push copy of "this" into stack as function call will pop it out.
					lValueDataType->PushReturn();
				}

				// Check for a "new" function.
				//Symbol* oldScope = _currentScope;
				//if (_currentPass == 2) _currentScope = newSymbol;
				ParseFunctionCall(newSymbol, true, "new", true);
				//if (_currentPass == 2) _currentScope = oldScope;
//			}
//			else
//			{
//				ExpectToken(TOKEN_CHAR_OPEN_PARENTHESIS);
//				ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS);
//			}

			break;
		}
		case TOKEN_CHAR_OPEN_PARENTHESIS:
		{
			lValueDataType = ParseExpression();
			ExpectToken(TOKEN_CHAR_CLOSE_PARENTHESIS); // TODO, Might have to be a check, not expect
			break;
		}
	}

	// Perform casting operation.
	//if (castExists == true)
	//{
	//	if (_currentPass == 2)
	//		lValueDataType->CastTo(castDataType);
	//	lValueDataType = castDataType;
	//}

	return lValueDataType;
}

// --------------------------------------------------------------------------------
//  This is placed in a seperate method just to shorten ParseExpressionLevel8 a bit
//  It reads the next token in the stream as an assignment operator, and then an 
//  expression, it then assigns the resulting value to the variable symbol. If
//  its a member variable, it expects the instance reference to be on the top
//  of the stack.
// --------------------------------------------------------------------------------
void Parser::ParseAssignment(VariableSymbol* variableSymbol)
{
	Instruction* instruction;
	TokenID operatorID;
	DataType* lValueDataType;

	// Check operator is not constant.
	if (_currentPass == 2 && (variableSymbol->VariableType == VT_CONST || variableSymbol->IsConst == true))
		g_app->EmitError("Attempt to assign value to \"" + variableSymbol->Identifier + "\", but variable is constant.", ES_PARSER, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);					

	// Store the operator token.
	operatorID = NextToken().ID;

	// Read in the expression.
	lValueDataType = ParseExpression();

	// Stack:
	//   this
	//   expression

	// Pop expression off stack.
	if (_currentPass == 2)
	{
		// Convert l-value to the same data type as as the variable.
		lValueDataType->CastTo(variableSymbol->DataType);
		lValueDataType = variableSymbol->DataType;

		// Perform operation if it's not a straight assign.
		if (operatorID != TOKEN_OP_ASSIGN)
		{
			// Perform assignment operator.
			switch (operatorID)
			{
				case TOKEN_OP_ASSIGN_ADD:
					{			
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpAdd();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_SUBTRACT:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpSubtract();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_MULTIPLY:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpMultiply();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_DIVIDE:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpDivide();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_MODULUS:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpBitwiseMOD();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_BITWISE_AND:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpBitwiseAND();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_BITWISE_OR:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpBitwiseOR();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_BITWISE_XOR:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpBitwiseXOR();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_BITWISE_NOT:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpBitwiseNOT();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_BITWISE_SHL:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpBitwiseSHL();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
				case TOKEN_OP_ASSIGN_BITWISE_SHR:
					{
						// Allocate a register to store the values in.
						Register lValueRegister = AllocateRegister(lValueDataType);

						// Pop expression value off stack.
						lValueDataType->Pop(lValueRegister);

						// Push variables value into stack.
						lValueDataType->PushVariableValue(variableSymbol);

						// Push expression value onto stack.
						lValueDataType->Push(lValueRegister);

						// Perform operation.
						lValueDataType = lValueDataType->OpBitwiseSHR();

						// Deallocate register.
						DeallocateRegister(lValueRegister);

						break;
					}
			}
		}

		// Store value in variable.
		lValueDataType->AssignToVariable(variableSymbol);
					
		// Push variables new value into stack.
		//lValueDataType->PushVariableValue(variableSymbol);

		lValueDataType = variableSymbol->DataType;
	}			
}
