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
//	Initializes the pre-processor.
// --------------------------------------------------------------------------------
PreProcessor::PreProcessor()
{
}

// --------------------------------------------------------------------------------
//	Makes the parser generate an instruction tree out of a token list.
// --------------------------------------------------------------------------------
bool PreProcessor::Parse(std::vector<Token> tokenList)
{	
	_tokenList = tokenList;
	_currentToken = Token();
	_tokenIndex = 0;

	while (!EndOfTokens())
	{
		NextToken();

		if (_currentToken.ID == TOKEN_CHAR_HASH)
		{
			std::string commandLit = NextToken().Literal;
			
			// Parse any if/if not defined macros.
			// These are parsed like C# defines, NO VALUES are allowed!
			//
			//		#ifdef identifier
			//		#ifndef identifier
			//
			if (commandLit == "ifdef" ||
				commandLit == "ifndef")
			{
				Token definedToken = NextToken();
				bool defined = false;
			
				// See if token is defined.
				for (int i = 0; i < _defineList.size(); i++)
					if (_defineList.at(i) == definedToken.Literal)
					{
						defined = true;
						break;
					}

				// If we are doing an if-not defined command then invert defined flag.
				if (commandLit == "ifndef")
					defined = !defined;

				// Add it to the if-block stack.
				_ifStateStack.push_back(defined);
			}

			// Parse the end of an if block.
			//
			//		#endif
			//
			else if (commandLit == "endif")
			{
				// Pop if state of the stack.
				if (_ifStateStack.size() > 0)
					_ifStateStack.pop_back();
				else
					g_app->EmitError("Pre-Processor found an #endif block with no matching #if block.", ES_PREPROCESSOR, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
			}

			
			// Parse any definition macros.
			// These are parsed like C# defines, NO VALUES are allowed!
			//
			//		#define identifier
			//
			else if (commandLit == "define")
			{
				Token definedToken = NextToken();
				_defineList.push_back(definedToken.Literal);
			}

			// Parse a file include statement.
			//
			//		#include path
			//
			else if (commandLit == "include")
			{
				Token includeToken = NextToken();

				// Get the base path to look from.
				std::string basePath = StringHelper::ExtractDir(_currentToken.Path);

				// Work out file path.
				std::string includePath = basePath + "\\" + includeToken.Literal;

				// Check to see if file exists.
				if (!FileHelper::FileExists(includePath))
					g_app->EmitError("Pre-Processor found an #include directive, but could not find source file declared (\"" + includePath + "\").", ES_PREPROCESSOR, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
			
				// Load in file contents.
				std::string fileContents = FileHelper::LoadText(includePath);

				// Run lexical analyser on file.
				LexicalAnalyser* lexer = new LexicalAnalyser();
				if (lexer->Analyse(fileContents, includePath) == true)
				{
					std::vector<Token> tokenList = lexer->GetTokenList();

					// Run pre-processor on file.
					PreProcessor* preprocessor = new PreProcessor();
					if (preprocessor->Parse(tokenList) == true)
					{
						tokenList = preprocessor->GetTokenList();
						
						printf(" - - Including '%s' (%i tokens) ...\n", StringHelper::StripDir(includePath).c_str(), tokenList.size());

						// Add tokens to the final token list.
						for (int i = 0; i < tokenList.size(); i++)
							_finalTokenList.push_back(tokenList.at(i));
					}
					delete preprocessor;
				}
				delete lexer;
			}

			// Unknown directive.
			else
			{
				g_app->EmitError("Pre-Processor found an unknown directive \"" + _currentToken.Literal + "\".", ES_PREPROCESSOR, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
			}

		}
		
		// Deals with any non-preprocessor tokens.
		else
		{
			// If the current if-state stack entry is true, or empty. Then
			// we can add this token to the final token list.
			if (_ifStateStack.size() == 0 ||
				_ifStateStack.at(_ifStateStack.size() - 1) == true)
			{
				// TODO: Go through definition list, if its in there, then replace
				// the token with given one.

				_finalTokenList.push_back(_currentToken);
			}
		}
	}
	
	return true;
}

// --------------------------------------------------------------------------------
//	Returns true if we are at the end of the token stream.
// --------------------------------------------------------------------------------
bool PreProcessor::EndOfTokens()
{
	return (_tokenIndex >= (int)_tokenList.size());
}

// --------------------------------------------------------------------------------
//	Returns the next token in the stream.
// --------------------------------------------------------------------------------
Token PreProcessor::NextToken()
{
	if (EndOfTokens()) 
		g_app->EmitError("An internal error occured. Pre-Processor attempted to read token past end of stream.", ES_PREPROCESSOR, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
	_currentToken = _tokenList.at(_tokenIndex++);

	return _currentToken;
}

// --------------------------------------------------------------------------------
//	Returns the token at current_position + offset without advancing the stream.
// --------------------------------------------------------------------------------
Token PreProcessor::LookAheadToken(int offset)
{
	offset--; // Remove one from the index seeing as tokenindex already 
			  // points to one token beyond our current one.
	if (_tokenIndex + offset >= (int)_tokenList.size())
		g_app->EmitError("An internal error occured. Pre-Processor expected look ahead token, but encountered end of file.", ES_PREPROCESSOR, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);

	return _tokenList.at(_tokenIndex + offset);
}

// --------------------------------------------------------------------------------
//	Reads in the next token and checks it has the given ID, if not it 
//  raises an error.
// --------------------------------------------------------------------------------
Token PreProcessor::ExpectToken(TokenID id)
{
	NextToken();
	CheckToken(id);
	return _currentToken;
}

// --------------------------------------------------------------------------------
//	Checks the current token, if it dosen't have the ID given it raises an error.
// --------------------------------------------------------------------------------
void PreProcessor::CheckToken(TokenID id)
{
	if (_currentToken.ID != id)
		g_app->EmitError("Encountered unexpected token \"" + _currentToken.Literal + "\".", ES_PREPROCESSOR, EL_FATAL, _currentToken.Line, _currentToken.Column, _currentToken.Path);
}
