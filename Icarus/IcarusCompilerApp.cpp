// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------

#include "stdafx.h"

// Array of strings used to define error sources.
std::string ErrorSource_Mnemonics[] = 
{
	"Lexer",
	"Parser",
	"Linker",
	"Assembler",
	"PreProcessor",
	""
};

// Array of strings used to define error levels.
std::string ErrorLevel_Mnemonics[] = 
{
	"Fatal Error",
	"Error",
	"Warning",
	"Info",
};

// --------------------------------------------------------------------------------
//	Parses the command lines given to work out what
//  and how should be compiled.
// --------------------------------------------------------------------------------
bool IcarusCompilerApp::ParseCommandLines(int argc, char* argv[])
{
	EmitOutputHeader();

	_executablePath = argv[0];

	for (int i = 1; i < argc; i++)
	{
		std::string argument = std::string(argv[i]);

		// Split the argument at the colon.
		std::string* colonSplit = new std::string[2];
		int colonSplitCount = StringHelper::Split(argument, ':', 2, colonSplit);

		// Defines the path to the input file to compile.
		// Syntax = /input:path
		if (colonSplit[0] == "/input")
		{
			_inputFile = colonSplit[1];
			
			// If its a relative path then add the base path to it.
			std::string basePath = StringHelper::ExtractDir(StringHelper::ExtractDir(_executablePath));
			if (_inputFile.find(':') == std::string::npos)
				basePath = basePath + "\\" + _inputFile;

			if (!FileHelper::FileExists(_inputFile))
			{
				EmitError("Input file does not exist '" + colonSplit[1] + "'.");
				return false;
			}
		}

		// Defines the path to the output file to compile to.
		// Syntax = /output:path
		else if (colonSplit[0] == "/output")
		{
			_outputFile = colonSplit[1];
		}

		// Defines the file format of the file to output.
		// Syntax = /format:pe|lib
		else if (colonSplit[0] == "/format")
		{
			if (colonSplit[1] == "pe")
				_compileFormat = CF_EXECUTABLE;
			else if (colonSplit[1] == "lib")
				_compileFormat = CF_LIBRARY;
			else
			{
				EmitError("Invalid compile format '" + colonSplit[1] + "'. For more information use use argument /?.");
				return false;
			}
		}

		// If set the output file will be a gui file not a console file.
		// Syntax = /gui
		else if (colonSplit[0] == "/gui")
		{
			_outputGUI = true;
		}

		// This argument will emit instructions on how
		// to use this application.
		// Syntax = /help|?
		else if (colonSplit[0] == "/help" ||
				 colonSplit[0] == "/?")
		{
			EmitOutputHelp();
		}

		// If parameter is anything else, then error out.
		else
		{	
			EmitError("Invalid command line argument '" + colonSplit[0] + "'. For more information use use argument /?.");
			return false;
		}

		// Tidy up the split array.
		delete[] colonSplit;
	}
	
	return true;
}

// --------------------------------------------------------------------------------
//	Compiles source code as specified in the command lines.
// --------------------------------------------------------------------------------
bool IcarusCompilerApp::Compile()
{
	_inGlobalCompileExceptionBlock = true;
	try
	{
		// Begin compilation on the input file.
		if (CompileFile(_inputFile) == false)
			return false;
	}
	catch (GlobalCompileException)
	{
		// Don't do anything, we just use this as a quick way 
		// of breaking out of our code.
	}
	_inGlobalCompileExceptionBlock = false;

	return true;
}

// --------------------------------------------------------------------------------
//	Starts compiling a given file.
// --------------------------------------------------------------------------------
bool IcarusCompilerApp::CompileFile(std::string path)
{
	bool status;

	DWORD startTime = GetTickCount();

	printf("Compiling File: %s\n", path.c_str());

	// Work out the location of the app-stub file.
	std::string basePath = StringHelper::ExtractDir(StringHelper::ExtractDir(_executablePath));
	std::string libPath = basePath + "\\lib";
	std::string appStubPath = libPath + "\\Headers\\AppStub.ic";
	if (!FileHelper::FileExists(appStubPath))
	{
		EmitError("Unable to access appstub function import descriptor file, unable to compile.");
		return false;
	}

	// Load in the contents of the file.
	printf(" - Loading contents ...\n");
	std::string contents = FileHelper::LoadText(path);
	std::string appStubContents = FileHelper::LoadText(appStubPath);

	// Start lexically analysing the code.
	printf(" - Lexically analysing %i characters ...\n", contents.size() + appStubContents.size());
	LexicalAnalyser* lexer = new LexicalAnalyser();
	if (lexer->Analyse(contents, path) == true)
	{
		std::vector<Token> sourceTokenList = lexer->GetTokenList();

		// Analyse the app-stub file to.
		if (lexer->Analyse(appStubContents, appStubPath) == true)
		{
			std::vector<Token> appStubTokenList = lexer->GetTokenList();

			// Combine token lists.		
			std::vector<Token> tokenList = appStubTokenList;
			for (int i = 0; i < sourceTokenList.size(); i++)
				tokenList.push_back(sourceTokenList.at(i));

			// Now we run the preprocessor over the token list and 
			// take care of any includes/ifdefs/etc.
			printf(" - Preprocessing %i tokens ...\n", tokenList.size());
			PreProcessor* _preprocessor = new PreProcessor();
			if (_preprocessor->Parse(tokenList) == true)
			{
				// Get our new token list.
				tokenList = _preprocessor->GetTokenList();

				// Now we have a tokenized output, lets try and turn this code
				// into an instruction tree.
				printf(" - Parsing %i tokens ...\n", tokenList.size());
				_parser = new Parser();
				if (_parser->Parse(tokenList) == true)
				{
					// Now we have an instruction tree, time to 
					// generate some code :3
					printf(" - Generating code ...\n");
					CodeGenerator* codeGen = new CodeGenerator();
					if (codeGen->Generate(_outputFile, _compileFormat, _outputGUI, _parser->GetGlobalScope()) == true)
					{
						int seconds = (GetTickCount() - startTime) / 1000;
						printf(std::string("\nCompilation complete (took "+StringHelper::IntToString(((seconds/60)/60)%60)+":"+StringHelper::IntToString((seconds/60)%60)+":"+StringHelper::IntToString(seconds%60)+"), output: %s\n").c_str(), _outputFile.c_str());
						status = true;
					}

					delete codeGen;
				}
				delete _parser;
			}
			delete _preprocessor;
		}
	}
	delete lexer;

	return status;
}

// --------------------------------------------------------------------------------
//	Emits an error to the standard output stream. If the error level is fatal
//  this will also thrown an exception which will unwind the callstack
//  back to the initial compile call.
// --------------------------------------------------------------------------------
void IcarusCompilerApp::EmitError(std::string description, 
								  ErrorSource source, 
								  ErrorLevel level, 
								  int line, 
								  int column,
								  std::string path)
{
	// Format:
	//   Linker Fatal Error (0:0) - There is an error in sub-section C of blah blah blah.  
	
	std::string sourceString = ErrorSource_Mnemonics[(int)source];
	std::string levelString  = ErrorLevel_Mnemonics[(int)level];
	std::string errorString = sourceString + (sourceString != "" ? " " : "") + levelString + " (" + path + "@" + StringHelper::IntToString(line) + ":" + StringHelper::IntToString(column) + ") - " + description;

	printf("%s\n", errorString.c_str());

	// If we are in the global complication exception block and we are fatal
	// then throw an error to quick escape out of our code.
	if (_inGlobalCompileExceptionBlock == true && level == EL_FATAL)
	{		
		throw GlobalCompileException();
	}
}

// --------------------------------------------------------------------------------
//  This just emits the copyright / information section at the top of the 
//  console output for this application.
// --------------------------------------------------------------------------------
void IcarusCompilerApp::EmitOutputHeader()
{
	printf("---------------------------------------------------------------\n");
	printf(" I c a r u s  C o m p i l e r (Version %s)\n", APP_VERSION);
	printf("---------------------------------------------------------------\n");
	printf(" Written By Timothy Leonard\n");
	printf(" Use /? or /help for command line arguments and instructions.\n");
	printf("---------------------------------------------------------------\n");
	printf("\n");
}

// --------------------------------------------------------------------------------
//  This just emits help information regarding the different command line
//  arguments and how to use this application.
// --------------------------------------------------------------------------------
void IcarusCompilerApp::EmitOutputHelp()
{
	printf("Command Line Arguments:\n");
	printf("Be aware that all command line arguments are in lower case, except for paths.\n");
	printf("\n");
	printf("  - /? /help\n");
	printf("    Help\n");
	printf("    Shows this message.\n");
	printf("\n");
	printf("  - /output:file\n");
	printf("    Output File\n");
	printf("    Defines which file the compiled output will be stored in.\n");
	printf("\n");
	printf("  - /input:file\n");
	printf("    Input File\n");
	printf("    Defines the source file to begin compliation from. \n");
	printf("\n");
	printf("  - /format:pe|dll\n");
	printf("    Output Format\n");
	printf("    Defines what kind of executable output this program should generate. \n");
	printf("    PE=Executable, LIB=Library File\n");
	printf("\n");
	printf("  - /gui\n");
	printf("    Output GUI\n");
	printf("    If set the output will be in GUI format not console format. \n");
	printf("\n");
}
