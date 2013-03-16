// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

class Parser;

// --------------------------------------------------------------------------------
//	Defines the what part of the application an error was emitted by.
// --------------------------------------------------------------------------------
enum ErrorSource
{
	ES_LEXER = 0,
	ES_PARSER = 1,
	ES_LINKER = 2,
	ES_ASSEMBLER = 3,
	ES_PREPROCESSOR = 4,
	ES_MISC = 5
};

// Defined in cpp, array of strings used to describe error sources.
extern std::string ErrorSource_Mnemonics[];

// --------------------------------------------------------------------------------
//	Defines the seriousness of an error that has occured.
// --------------------------------------------------------------------------------
enum ErrorLevel
{
	EL_FATAL = 0,
	EL_ERROR = 1,
	EL_WARNING = 2,
	EL_INFO = 3,
};

// Defined in cpp, array of strings used to describe error levels.
extern std::string ErrorLevel_Mnemonics[];

// --------------------------------------------------------------------------------
//	This is a class is just used as a way of jumping out
//  of our code execution quickly.
// --------------------------------------------------------------------------------
struct GlobalCompileException
{

};

// --------------------------------------------------------------------------------
//	This enumeration defines the format that we should output
//  our compiled code in.
// --------------------------------------------------------------------------------
enum CompileFormat
{
	CF_EXECUTABLE,
	CF_LIBRARY
};

// --------------------------------------------------------------------------------
//	Main application class, deals with taking in the command lines given
//  and compiling according to them.
// --------------------------------------------------------------------------------
class IcarusCompilerApp
{
private:
	bool _inGlobalCompileExceptionBlock;

	CompileFormat _compileFormat;
	std::string _inputFile;
	std::string _outputFile;

	std::string _executablePath;

	Parser* _parser;

	bool _outputGUI;

public:
	void EmitOutputHeader();
	void EmitOutputHelp();

	bool ParseCommandLines(int argc, char* argv[]);
	bool Compile();
	bool CompileFile(std::string value);

	void EmitError(std::string description, ErrorSource source = ES_MISC, ErrorLevel level = EL_FATAL, int line = 0, int column = 0, std::string path = "UNDEFINED");

	Parser* GetParser() { return _parser; }
	std::string GetExecutablePath() { return _executablePath; }

};