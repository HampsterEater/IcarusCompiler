// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	The preprocessor is responsible for taking a list of tokens and processing
//  and pre-processor commands it contains.
// --------------------------------------------------------------------------------
class PreProcessor
{
private:
	std::vector<Token> _tokenList;
	std::vector<Token> _finalTokenList;

	std::vector<std::string> _defineList;
	std::vector<bool> _ifStateStack;

	Token _currentToken;
	int _tokenIndex;
	
public:
	PreProcessor();

	bool Parse(std::vector<Token> tokenList);

	std::vector<Token> GetTokenList() { return _finalTokenList; }

	bool EndOfTokens();
	Token NextToken();
	Token LookAheadToken(int offset=1);
	Token ExpectToken(TokenID id);
	void CheckToken(TokenID id);
};