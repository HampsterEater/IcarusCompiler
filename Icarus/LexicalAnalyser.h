// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	Defines the numeric identification of each individual 
//  type of token that a source file can contain.
// --------------------------------------------------------------------------------
enum TokenID
{
	TOKEN_LITERAL_IDENTIFIER,
	TOKEN_LITERAL_STRING,
	TOKEN_LITERAL_FLOAT,
	TOKEN_LITERAL_DOUBLE,
	TOKEN_LITERAL_INTEGER,
	
	TOKEN_OP_SCOPE_RESOLUTION,
	TOKEN_OP_INDIRECTION,	

	TOKEN_OP_MODULUS,				
	TOKEN_OP_LOGICAL_AND,			
	TOKEN_OP_LOGICAL_OR,			
	TOKEN_OP_LOGICAL_NOT,			

	TOKEN_OP_BITWISE_SHL,			
	TOKEN_OP_BITWISE_SHR,			
	TOKEN_OP_BITWISE_AND,			
	TOKEN_OP_BITWISE_OR,			
	TOKEN_OP_BITWISE_XOR,			
	TOKEN_OP_BITWISE_NOT,			

	TOKEN_OP_MULTIPLY,				
	TOKEN_OP_DIVIDE,				
	TOKEN_OP_SUBTRACT,					
	TOKEN_OP_ADD,					

	TOKEN_OP_LESS,					
	TOKEN_OP_GREATER,				
	TOKEN_OP_LESS_EQUAL,			
	TOKEN_OP_GREATER_EQUAL,			
	TOKEN_OP_NOT_EQUAL,				
	TOKEN_OP_EQUAL,				

	TOKEN_OP_ASSIGN,				
	TOKEN_OP_ASSIGN_ADD,			
	TOKEN_OP_ASSIGN_SUBTRACT,			
	TOKEN_OP_ASSIGN_MULTIPLY,		
	TOKEN_OP_ASSIGN_DIVIDE,			
	TOKEN_OP_ASSIGN_MODULUS,		
	TOKEN_OP_ASSIGN_BITWISE_AND,		
	TOKEN_OP_ASSIGN_BITWISE_OR,		
	TOKEN_OP_ASSIGN_BITWISE_XOR,		
	TOKEN_OP_ASSIGN_BITWISE_NOT,		
	TOKEN_OP_ASSIGN_BITWISE_SHL,		
	TOKEN_OP_ASSIGN_BITWISE_SHR,		
	TOKEN_OP_INCREMENT,			
	TOKEN_OP_DECREMENT,			

	TOKEN_CHAR_OPEN_BRACE,		
	TOKEN_CHAR_CLOSE_BRACE,			
	TOKEN_CHAR_OPEN_BRACKET,		
	TOKEN_CHAR_CLOSE_BRACKET,		
	TOKEN_CHAR_OPEN_PARENTHESIS,	
	TOKEN_CHAR_CLOSE_PARENTHESIS,	

	TOKEN_CHAR_COMMA,	
	TOKEN_CHAR_HASH,	
	TOKEN_CHAR_AT,			
	TOKEN_CHAR_COLON,				
	TOKEN_CHAR_SEMICOLON,			
	TOKEN_CHAR_PERIOD,			

	TOKEN_KEYWORD_METHOD,

	TOKEN_KEYWORD_DOUBLE,  
	TOKEN_KEYWORD_FLOAT,  
	TOKEN_KEYWORD_INT,       
	TOKEN_KEYWORD_SHORT,  
	TOKEN_KEYWORD_LONG,       
	TOKEN_KEYWORD_STRING,
	TOKEN_KEYWORD_BYTE,   
	TOKEN_KEYWORD_VOID,
	TOKEN_KEYWORD_STRUCT,  
	TOKEN_KEYWORD_OBJECT,
	TOKEN_KEYWORD_BOOL,  
	TOKEN_KEYWORD_GLOBAL,
	TOKEN_KEYWORD_CONST,   
	TOKEN_KEYWORD_UNSIGNED, 
	TOKEN_KEYWORD_SIGNED,  
	TOKEN_KEYWORD_ARRAY,
	TOKEN_KEYWORD_TO,
	TOKEN_KEYWORD_ABSTRACT,
	TOKEN_KEYWORD_ALWAYS,
	TOKEN_KEYWORD_WITHIN,
	TOKEN_KEYWORD_UNTIL,
	TOKEN_KEYWORD_UNRELIABLE,
	TOKEN_KEYWORD_TRANSIENT,
	TOKEN_KEYWORD_SUPER,
	TOKEN_KEYWORD_STOP,
	TOKEN_KEYWORD_INSERT,
	TOKEN_KEYWORD_LOCAL,
	TOKEN_KEYWORD_LOCALIZED,
	TOKEN_KEYWORD_SELF,
	TOKEN_KEYWORD_SINGULAR,
	TOKEN_KEYWORD_SKIP,
	TOKEN_KEYWORD_STATE,
	TOKEN_KEYWORD_LATENT,
	TOKEN_KEYWORD_INTRINSIC,
	TOKEN_KEYWORD_INPUT,
	TOKEN_KEYWORD_EXEC,
	TOKEN_KEYWORD_IGNORES,
	TOKEN_KEYWORD_NONE,
	TOKEN_KEYWORD_OUT,
	TOKEN_KEYWORD_CONFIG,
	TOKEN_KEYWORD_ASSERT,
	TOKEN_KEYWORD_AUTO,
	TOKEN_KEYWORD_EVENT, 
	TOKEN_KEYWORD_FINAL,
	TOKEN_KEYWORD_BREAK, 
	TOKEN_KEYWORD_CONTINUE,  
	TOKEN_KEYWORD_ELSE,    
	TOKEN_KEYWORD_FOR,    
	TOKEN_KEYWORD_SWITCH,   
	TOKEN_KEYWORD_CASE,   
	TOKEN_KEYWORD_DEFAULT,   
	TOKEN_KEYWORD_DELEGATE,
	TOKEN_KEYWORD_DEPENDSON,
	TOKEN_KEYWORD_DEPRECATED,
	TOKEN_KEYWORD_ENUM,    
	TOKEN_KEYWORD_GOTO,   
	TOKEN_KEYWORD_REGISTER,   
	TOKEN_KEYWORD_VOLATILE,
	TOKEN_KEYWORD_DO,        
	TOKEN_KEYWORD_EXTERN,  
	TOKEN_KEYWORD_IF,     
	TOKEN_KEYWORD_RETURN,    
	TOKEN_KEYWORD_STATIC,  
	TOKEN_KEYWORD_UNION,   
	TOKEN_KEYWORD_WHILE,
	TOKEN_KEYWORD_VAR,
	TOKEN_KEYWORD_NULL,
	TOKEN_KEYWORD_FUNCTION,
	TOKEN_KEYWORD_END,
	TOKEN_KEYWORD_EACHIN,
	TOKEN_KEYWORD_NAME,
	TOKEN_KEYWORD_ASM,         
	TOKEN_KEYWORD_NAMESPACE,  
	TOKEN_KEYWORD_TRY,      
	TOKEN_KEYWORD_EXPLICIT,      
	TOKEN_KEYWORD_NEW,             
	TOKEN_KEYWORD_TYPEID,
	TOKEN_KEYWORD_CATCH,   
	TOKEN_KEYWORD_FINALLY,       
	TOKEN_KEYWORD_FALSE,         
	TOKEN_KEYWORD_OPERATOR,   
	TOKEN_KEYWORD_TEMPLATE,   
	TOKEN_KEYWORD_CLASS,   
	TOKEN_KEYWORD_FRIEND,        
	TOKEN_KEYWORD_PRIVATE,    
	TOKEN_KEYWORD_THIS,              
	TOKEN_KEYWORD_USING, 
	TOKEN_KEYWORD_INLINE,       
	TOKEN_KEYWORD_PUBLIC,    
	TOKEN_KEYWORD_THROW,             
	TOKEN_KEYWORD_VIRTUAL,
	TOKEN_KEYWORD_DELETE,      
	TOKEN_KEYWORD_MUTABLE,       
	TOKEN_KEYWORD_PROTECTED,  
	TOKEN_KEYWORD_TRUE,    
	TOKEN_KEYWORD_AUTOMATED,  
	TOKEN_KEYWORD_COERCE,

	TOKEN_KEYWORD_GENERATOR,  
	TOKEN_KEYWORD_YIELD,
	TOKEN_KEYWORD_FOREACH,
	TOKEN_KEYWORD_IN

};

// --------------------------------------------------------------------------------
//	This is a simple structure it just stores a token keyword, which is used
//  to work out which identifiers should be turned into keywords.
// --------------------------------------------------------------------------------
struct TokenKeyword
{
public:
//private:
	std::string Literal;

	TokenID KeywordID;
	std::string KeywordLiteral;


//public:
	TokenKeyword(TokenID keywordID, std::string lit, std::string keywordLiteral="")
	{
		Literal = lit;
		KeywordID = keywordID;
		KeywordLiteral = keywordLiteral;
	}
	TokenKeyword()
	{
		// Default constructor.
	} 
};

// --------------------------------------------------------------------------------
//	This structure is used to store information on an individual
//  token segment within the code source. (a tokens are the segments made
//  when you logically break up the code, eg. ++ -- > < if while when, etc).
// --------------------------------------------------------------------------------
struct Token
{
public:
//private:
	TokenID ID;
	std::string Literal;

	std::string Path;
	int Line;
	int Column;
	std::string LineLiteral;

//public:
	Token(TokenID id, std::string lit, std::string path, int line, int column, std::string lineLiteral = "")
	{
		ID = id;
		Literal = lit;
		Path = path;
		Line = line;
		Column = column;
		LineLiteral = lineLiteral;
	}
	Token()
	{
		// Default constructor.
	} 
};

// --------------------------------------------------------------------------------
//	Lexical analyser responsible for turning a source file into a list of tokens.
// --------------------------------------------------------------------------------
class LexicalAnalyser
{
private:
	std::vector<TokenKeyword> _tokenKeywords;

	std::vector<Token> _tokenList;
	std::string _text;

	int _position, _offset, _line;
	std::string _file;

	char _currentChar;

	std::string _currentLineLiteral;

public:
	LexicalAnalyser();
	
	std::vector<Token> GetTokenList();

	void ReadNextLine();

	bool Analyse(std::string data, std::string path);

	bool EndOfText();
	char NextChar();
	char LookAheadChar();
	bool NextToken(Token* token);
	bool LexToken(Token* token);

	bool LexString(Token* token);
	bool LexIdentifier(Token* token);
	bool LexNumber(Token* token);

};