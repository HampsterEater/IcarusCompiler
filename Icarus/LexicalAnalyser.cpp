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
//	Initializes the lexical analyser.
// --------------------------------------------------------------------------------
LexicalAnalyser::LexicalAnalyser()
{
	// Setup list of token keywords.
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_METHOD, "method")); 
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_DOUBLE, "double"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_FLOAT, "float"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_INT, "int"));         
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_SHORT, "short"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_LONG, "long"));         
	//_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_STRING, "string"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_BYTE, "byte"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_VOID, "void"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_STRUCT, "struct"));    
	//_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_OBJECT, "object"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_BOOL, "bool"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_GLOBAL, "global"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_CONST, "const"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_UNSIGNED, "unsigned"));   
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_SIGNED, "signed"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_ARRAY, "array"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_TO, "to"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_ABSTRACT, "abstract"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_ALWAYS, "always"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_WITHIN, "within"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_UNTIL, "until"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_UNRELIABLE, "unreliable"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_TRANSIENT, "transient"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_SUPER, "super"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_STOP, "stop"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_INSERT, "insert"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_LOCAL, "local"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_LOCALIZED, "localized"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_SELF, "self"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_SINGULAR, "singular"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_SKIP, "skip"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_STATE, "state"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_LATENT, "latent"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_INTRINSIC, "intrinsic"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_INPUT, "input"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_EXEC, "exec"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_IGNORES, "ignores"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_NONE, "none"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_OUT, "out"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_CONFIG, "config"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_ASSERT, "assert"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_AUTO, "auto"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_EVENT, "event"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_FINAL, "final"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_FINALLY, "finally"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_BREAK, "break"));   
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_CONTINUE, "continue"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_ELSE, "else"));      
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_FOR, "for"));      
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_SWITCH, "switch"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_CASE, "case"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_DEFAULT, "default"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_DELEGATE, "delegate"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_DEPENDSON, "dependson"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_DEPRECATED, "deprecated"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_ENUM, "enum"));      
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_GOTO, "goto"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_REGISTER, "register"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_VOLATILE, "volatile"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_DO, "do"));          
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_EXTERN, "extern"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_IF, "if"));       
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_RETURN, "return"));      
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_STATIC, "static"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_UNION, "union"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_WHILE, "while"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_VAR, "var"));   
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_FUNCTION, "function"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_END, "end"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_EACHIN, "eachin"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_NAME, "name"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_ASM, "asm"));           
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_NAMESPACE, "namespace"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_TRY, "try"));        
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_EXPLICIT, "explicit"));        
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_NEW, "new"));               
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_TYPEID, "typeid"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_CATCH, "catch"));           
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_OPERATOR, "operator"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_TEMPLATE, "template"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_CLASS, "class"));     
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_FRIEND, "friend"));          
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_PRIVATE, "private"));      
	//_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_THIS, "this"));                
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_USING, "using"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_INLINE, "inline"));         
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_PUBLIC, "public"));      
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_THROW, "throw"));               
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_VIRTUAL, "virtual"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_DELETE, "delete"));        
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_MUTABLE, "mutable"));         
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_PROTECTED, "protected"));   
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_AUTOMATED, "automated"));   
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_COERCE, "coerce"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_GENERATOR, "generator"));    
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_YIELD, "yield"));  
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_FOREACH, "foreach"));
	_tokenKeywords.push_back(TokenKeyword(TOKEN_KEYWORD_IN, "in"));

	_tokenKeywords.push_back(TokenKeyword(TOKEN_LITERAL_INTEGER, "true", "1")); 
	_tokenKeywords.push_back(TokenKeyword(TOKEN_LITERAL_INTEGER, "false", "0"));
	_tokenKeywords.push_back(TokenKeyword(TOKEN_LITERAL_INTEGER, "null", "0")); 
}

// --------------------------------------------------------------------------------
//	Returns the current token list.
// --------------------------------------------------------------------------------
std::vector<Token> LexicalAnalyser::GetTokenList()
{
	return _tokenList;
}

// --------------------------------------------------------------------------------
//	Reads in the next line of text and places it in _
// --------------------------------------------------------------------------------
void LexicalAnalyser::ReadNextLine()
{
	_currentLineLiteral = "";
	for (int i = _position + 1; i < (int)_text.length(); i++)
	{
		if (_text.at(i) == '\n')
			break;
		_currentLineLiteral += _text.at(i);
	}
}

// --------------------------------------------------------------------------------
//	Takes a string of data and converts it into a list of 
//  individual character tokens.
// --------------------------------------------------------------------------------
bool LexicalAnalyser::Analyse(std::string data, std::string path)
{	
	Token token; 

	// Initialize all the members.
	_text = data;
	_file = path;
	_position = -1;
	_offset = 0;
	_line = 1;
	_currentChar = '\0';
	_tokenList.clear();
	_currentLineLiteral = "";

	// Get everything started by reading in the first character.
	ReadNextLine();
	NextChar();
	
	// Keep lexing tokens until we are at the end
	// of the text stream.
	while (!EndOfText())
	{
		if (NextToken(&token) == true)
			_tokenList.push_back(token);		
	}

	return true;
}

// --------------------------------------------------------------------------------
//  Returns true if we are at the end of the text stream.
// --------------------------------------------------------------------------------
bool LexicalAnalyser::EndOfText()
{
	return (int)_position >= (int)_text.length();
}

// --------------------------------------------------------------------------------
//  Returns the next character in the stream.
// --------------------------------------------------------------------------------
char LexicalAnalyser::NextChar()
{
	// Extract the character from the text.
	if ((int)_position + 1 < (int)_text.length())
		_currentChar = _text.at(_position + 1);
	else
		_currentChar = '\0';
		
	// Increment positions.
	_position++;
	_offset++;

	// Check for new lines (to track line numbers).
	if (_currentChar == '\n')
	{
		ReadNextLine();
		_line++;
		_offset = 0;
	}
	
	return _currentChar;
}

// --------------------------------------------------------------------------------
//  Returns the look ahead char.
// --------------------------------------------------------------------------------
char LexicalAnalyser::LookAheadChar()
{
	char lac;

	// Extract the character from the text.
	if ((int)_position + 1 < (int)_text.length())
		lac = _text.at(_position + 1);
	else
		lac = '\0';
	
	return lac;
}

// --------------------------------------------------------------------------------
//  Returns the next token in the stream.
// --------------------------------------------------------------------------------
bool LexicalAnalyser::NextToken(Token* token)
{
	bool retVal;

	// Check we are not at the end of the text stream.
	if (EndOfText() == true)
		return false;

	// Keep parsing until we either return a valid
	// token or get to the end of the text.
	while ((retVal = LexToken(token)) == false && 
		   EndOfText() == false)
		; // Empty statement.

	return retVal;
}

// --------------------------------------------------------------------------------
//  Lexically analyses the next token in the stream.
// --------------------------------------------------------------------------------
bool LexicalAnalyser::LexToken(Token* token)
{
	bool foundToken;

	foundToken = false;

	// Parse next token depending on current character.
	switch (_currentChar)
	{	
		// -----------------------------------------------------------------
		// Assignment.
		// -----------------------------------------------------------------
		case '=':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_EQUAL, "==", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_ASSIGN, "=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}

		// -----------------------------------------------------------------
		// XOr
		// -----------------------------------------------------------------
		case '^':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_ASSIGN_BITWISE_XOR, "^=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_BITWISE_XOR, "^", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}

		// -----------------------------------------------------------------
		// Bitwize Not
		// -----------------------------------------------------------------
		case '~':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_ASSIGN_BITWISE_NOT, "~=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_BITWISE_NOT, "~", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}
		
		// -----------------------------------------------------------------
		// Bitwize Or
		// -----------------------------------------------------------------
		case '|':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_ASSIGN_BITWISE_OR, "|=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				case '|':
				{
					NextChar();
					*token = Token(TOKEN_OP_LOGICAL_OR, "||", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_BITWISE_OR, "|", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}
		
		// -----------------------------------------------------------------
		// Bitwize And
		// -----------------------------------------------------------------
		case '&':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_ASSIGN_BITWISE_AND, "&=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				case '&':
				{
					NextChar();
					*token = Token(TOKEN_OP_LOGICAL_AND, "&&", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_BITWISE_AND, "&", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}

		// -----------------------------------------------------------------
		// Modulus
		// -----------------------------------------------------------------
		case '%':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_ASSIGN_MODULUS, "%=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_MODULUS, "%", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}

		// -----------------------------------------------------------------
		// Addition
		// -----------------------------------------------------------------
		case '+':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_ASSIGN_ADD, "+=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				case '+':
				{
					NextChar();
					*token = Token(TOKEN_OP_INCREMENT, "++", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_ADD, "+", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}
		
		// -----------------------------------------------------------------
		// Mupltiply
		// -----------------------------------------------------------------
		case '*':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_ASSIGN_MULTIPLY, "*=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_MULTIPLY, "*", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}
				
		// -----------------------------------------------------------------
		// Not
		// -----------------------------------------------------------------
		case '!':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_NOT_EQUAL, "!=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_LOGICAL_NOT, "!", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}

		// -----------------------------------------------------------------
		// Less Than Comparison
		// -----------------------------------------------------------------
		case '<':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_LESS_EQUAL, "<=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				case '<':
				{
					switch (NextChar())
					{
						case '=':
						{
							NextChar();
							*token = Token(TOKEN_OP_ASSIGN_BITWISE_SHL, "<<=", _file, _line, _offset, _currentLineLiteral);
							foundToken = true;
							break;
						}
						default:
						{
							*token = Token(TOKEN_OP_BITWISE_SHL, "<<", _file, _line, _offset, _currentLineLiteral);
							foundToken = true;
							break;
						}
					}
					break;
				}
				case '>':
				{
					NextChar();
					*token = Token(TOKEN_OP_NOT_EQUAL, "<>", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_LESS, "<", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}
		
		// -----------------------------------------------------------------
		// Greater Than Comparison
		// -----------------------------------------------------------------
		case '>':
		{
			switch (NextChar())
			{
				case '=':
				{
					NextChar();
					*token = Token(TOKEN_OP_GREATER_EQUAL, ">=", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				case '>':
				{
					switch (NextChar())
					{
						case '=':
						{
							NextChar();
							*token = Token(TOKEN_OP_ASSIGN_BITWISE_SHR, ">>=", _file, _line, _offset, _currentLineLiteral);
							foundToken = true;
							break;
						}
						default:
						{
							*token = Token(TOKEN_OP_BITWISE_SHR, ">>", _file, _line, _offset, _currentLineLiteral);
							foundToken = true;
							break;
						}
					}
					break;
				}
				case '<':
				{
					NextChar();
					*token = Token(TOKEN_OP_NOT_EQUAL, "><", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_OP_GREATER, ">", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}
		
		// -----------------------------------------------------------------
		// Single Characters
		// -----------------------------------------------------------------
		case '{':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_OPEN_BRACE, "{", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case '}':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_CLOSE_BRACE, "}", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case '[':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_OPEN_BRACKET, "[", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case ']':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_CLOSE_BRACKET, "]", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case '(':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_OPEN_PARENTHESIS, "(", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case ')':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_CLOSE_PARENTHESIS, ")", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case ',':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_COMMA, ",", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case '#':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_HASH, "#", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case ':':
		{
			switch (NextChar())
			{
				case ':':
				{
					NextChar();
					*token = Token(TOKEN_OP_SCOPE_RESOLUTION, "::", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
				default:
				{
					*token = Token(TOKEN_CHAR_COLON, ":", _file, _line, _offset, _currentLineLiteral);
					foundToken = true;
					break;
				}
			}
			break;
		}
		case ';':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_SEMICOLON, ";", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case '.':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_PERIOD, ".", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}
		case '@':
		{
			NextChar();
			*token = Token(TOKEN_CHAR_AT, "@", _file, _line, _offset, _currentLineLiteral);
			foundToken = true;
			break;
		}

		// -----------------------------------------------------------------
		// Whitespace?
		// -----------------------------------------------------------------
		case ' ':
		case '\n':
		case '\r':
		case '\t':
		{
			NextChar();
			break;
		}

		// -----------------------------------------------------------------
		// Negative number, subtract, etc.
		// -----------------------------------------------------------------
		case '-':
		{
			char lac = LookAheadChar();
			
			// Is it a number?
			if (lac >= '0' && lac <= '9')
			{
				foundToken = LexNumber(token);
			}
			else
			{
				switch (NextChar())
				{
					// Subtract assign.
					case '=':
					{
						NextChar();
						*token = Token(TOKEN_OP_ASSIGN_SUBTRACT, "-=", _file, _line, _offset, _currentLineLiteral);
						foundToken = true;
						break;
					}
				
					// Decrement.
					case '-':
					{
						NextChar();
						*token = Token(TOKEN_OP_DECREMENT, "--", _file, _line, _offset, _currentLineLiteral);
						foundToken = true;
						break;
					}
			
					// Indirection.
					case '>':
					{
						NextChar();
						*token = Token(TOKEN_OP_INDIRECTION, "->", _file, _line, _offset, _currentLineLiteral);
						foundToken = true;
						break;
					}
					
					// Subtract.
					default:
					{
						*token = Token(TOKEN_OP_SUBTRACT, "-", _file, _line, _offset, _currentLineLiteral);
						foundToken = true;
						break;
					}
				}
			}
			break;
		}
		
		// -----------------------------------------------------------------
		// Comment / Division / etc
		// -----------------------------------------------------------------
		case '/':
			{
				switch (NextChar())
				{
					// Single line comment.
					case '/':
					{
						// Keep reading until we get a new line character
						// or if we get to the end of the text.
						while (_currentChar != '\n' && EndOfText() == false)
							NextChar();

						break;
					}

					// Multi line comment.
					case '*':
					{
						char prevChar = _currentChar;
						int commentNest = 0;

						// Keep parsing until we get to the end of the comment.
						while (true)
						{
							// Are we at the end of the text?
							if (EndOfText())
								g_app->EmitError("Encountered unexpected end of file. Expecting block comment end.", ES_LEXER, EL_FATAL, _line, _offset, _file);

							// Read the next token in the character.
							NextChar();

							// Increase nesting?
							if (prevChar == '/' && _currentChar == '*')
								commentNest++;

							// Decrease nesting?
							if (prevChar == '*' && _currentChar == '/')
							{
								commentNest--;

								// If the nesting is zero, then it's time
								// to break out of the loop.
								if (commentNest < 0)
								{
									NextChar();
									break;
								}
							}

							prevChar = _currentChar;
						}

						break;
					}

					// Division assignment.
					case '=':
					{
						NextChar();
						*token = Token(TOKEN_OP_ASSIGN_DIVIDE, "/=", _file, _line, _offset, _currentLineLiteral);
						foundToken = true;
						break;
					}

					// Just a division.
					default:
					{
						*token = Token(TOKEN_OP_DIVIDE, "/", _file, _line, _offset, _currentLineLiteral);
						foundToken = true;
						break;
					}
				}
				break;
			}

		// -----------------------------------------------------------------
		// String literals.
		// -----------------------------------------------------------------
		case '"':
		case '\'':
		{
			foundToken = LexString(token);	
			break;
		}
		
		// -----------------------------------------------------------------
		// Anything else. 
		// Identifiers, Literals, Etc.
		// -----------------------------------------------------------------
		default:
		{
			// Is it an identifier?
			if ((_currentChar >= 'A' && _currentChar <= 'Z') || 
				(_currentChar >= 'a' && _currentChar <= 'z') ||
				_currentChar == '_')
			{
				foundToken = LexIdentifier(token);	
			}

			// Is it a number?
			else if (_currentChar >= '0' && _currentChar <= '9')
			{
				foundToken = LexNumber(token);
			}
			
			// WTF is this!?
			else
			{
				g_app->EmitError((std::string("Encountered unexpected chracter '") + _currentChar + "'."), ES_LEXER, EL_FATAL, _line, _offset, _file);
			}

			break;
		}
			
	}

	return foundToken;
}

// --------------------------------------------------------------------------------
//  Lexically parses a literal string.
// --------------------------------------------------------------------------------
bool LexicalAnalyser::LexString(Token* token)
{
	std::string fullLiteral;
	char startChar = _currentChar;

	// Keep reading chars till we get to
	// the end of the string.
	while (true)
	{
		// Found start character? We're at the end then. 
		if (NextChar() == startChar)
			break;

		// Are we at the end of the text?
		if (EndOfText() == true)
			g_app->EmitError("Encountered unexpected end of file. Expecting string literal end.", ES_LEXER, EL_FATAL, _line, _offset, _file);

		// Escape sequences.
		if (_currentChar == '\\')
		{
			switch (NextChar())
			{
				case '"': 
				case '\'': 
				case '\\':
				{
					fullLiteral += _currentChar; 
					break;
				}
				case 'n': 
				{
					fullLiteral += '\n'; 
					break;
				}
				case 'r': 
				{
					fullLiteral += '\r'; 
					break;
				}
				case 't': 
				{
					fullLiteral += '\t'; 
					break;
				}
				case '0': 
				{
					fullLiteral += '\0'; 
					break;
				}
				default:
				{
					g_app->EmitError("Encountered unknown escape sequence.", ES_LEXER, EL_FATAL, _line, _offset, _file);
					break;
				}
			}
		}

		// Normal character, append it.
		else
			fullLiteral += _currentChar;
	}
			
	NextChar();
	*token = Token(TOKEN_LITERAL_STRING, fullLiteral, _file, _line, _offset, _currentLineLiteral);

	return true;
}

// --------------------------------------------------------------------------------
//  Lexically parses a literal identifier.
// --------------------------------------------------------------------------------
bool LexicalAnalyser::LexIdentifier(Token* token)
{
	std::string fullLiteral;
	char startChar = _currentChar;
	int index = 0;
	TokenID id = TOKEN_LITERAL_IDENTIFIER;
	
	std::string lineLiteral = _currentLineLiteral;
	int lineIndex = _line;
	int lineOffset = _offset;

	// Keep reading chars till we get to
	// the end of the string.
	while (true)
	{
		// Append the next character.
		fullLiteral += _currentChar;

		// Read in the next character.
		NextChar();

		// Are we at the end of the text?
		if (EndOfText())
			break;

		// Is it a valid character?
		if (!(  (_currentChar >= 'A' && _currentChar <= 'Z') ||
			    (_currentChar >= 'a' && _currentChar <= 'z') ||
			    (_currentChar >= '0' && _currentChar <= '9') ||
			    _currentChar == '_' ))
			break;

		index++;
	}

	// Is it a keyword?
	for (int i = 0; i < (int)_tokenKeywords.size(); i++)
	{
		TokenKeyword keyword = _tokenKeywords.at(i);
		if (StringHelper::ToLower(keyword.Literal) == StringHelper::ToLower(fullLiteral))
		{
			id = keyword.KeywordID;
			if (keyword.KeywordLiteral != "")
				fullLiteral = keyword.KeywordLiteral;
		}
	}

	*token = Token(id, fullLiteral, _file, lineIndex, lineOffset, lineLiteral);
	return true;
}

// --------------------------------------------------------------------------------
//  Lexically parses a literal number (float, int, hex, etc).
// --------------------------------------------------------------------------------
bool LexicalAnalyser::LexNumber(Token* token)
{
	TokenID id = TOKEN_LITERAL_INTEGER;
	std::string fullLiteral;
	int index = 0;
	bool isHex = false;
	bool foundRadix = false;
	bool hasPostfix = true;
	char postfixChar = '\0';
	
	std::string lineLiteral = _currentLineLiteral;
	int lineIndex = _line;
	int lineOffset = _offset;

	// Keep reading chars till we get to
	// the end of the snumber.
	while (true)
	{
		// Hex prefix.
		if (_currentChar == 'x' || _currentChar == 'X')
		{
			if (index == 1 && fullLiteral.at(0) == '0' && isHex == false)
				isHex = true;
			else
				g_app->EmitError("Encountered malformed hexidecimal literal.", ES_LEXER, EL_FATAL, _line, _offset, _file);	
		}

		// Hex digit.
		else if ((_currentChar >= 'A' && _currentChar <= 'Z') || 
				 (_currentChar >= 'a' && _currentChar <= 'z'))
		{
			if (isHex == false)
			{
				if (_currentChar == 'f' || _currentChar == 'd')
				{
					hasPostfix = true;
					postfixChar = _currentChar;
					NextChar();
					//NextChar();
					break;
				}
				else
					g_app->EmitError("Encountered malformed numeric literal.", ES_LEXER, EL_FATAL, _line, _offset, _file);
			}
		}
		
		// Normal digit.
		else if ((_currentChar >= '0' && _currentChar <= '9'))
		{
			// Don't need to do anything here.
		}

		// Radix point?
		else if (_currentChar == '.')
		{
			if (foundRadix == true)
				g_app->EmitError("Encountered malformed numeric literal, multiple radix points found.", ES_LEXER, EL_FATAL, _line, _offset, _file);	
			else
				foundRadix = true;
		}

		// Negative?
		else if (_currentChar == '-')
		{
			if (index != 0)
				g_app->EmitError("Encountered malformed numeric literal, found sign at index other than 0.", ES_LEXER, EL_FATAL, _line, _offset, _file);		
		}

		// Invalid character?
		else
		{
			//NextChar();
			//_position -= 2;
			//NextChar();
			break;
		}

		// Append the next character.
		fullLiteral += _currentChar;

		// Read in the next character.
		NextChar();

		// Are we at the end of the text?
		if (EndOfText())
			break;

		index++;
	}

	// Radix?
	if (foundRadix == true)
	{
		id = TOKEN_LITERAL_FLOAT;
	}

	// Integer?
	if (hasPostfix == true)
	{
		// Is it float?
		if (postfixChar == 'f')
		{
			id = TOKEN_LITERAL_FLOAT;
		}

		// Is it double?
		else if (postfixChar  == 'd')
		{
			id = TOKEN_LITERAL_DOUBLE;
		}
	}

	*token = Token(id, fullLiteral, _file, lineIndex, lineOffset, lineLiteral);
	return true;
}