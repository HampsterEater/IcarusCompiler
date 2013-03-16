// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	The code generator is responsible for taking an instruction
//  tree and converting it into an executable file.
// --------------------------------------------------------------------------------
class CodeGenerator
{
private:
	std::vector<std::string> _linkLibraries;

	FunctionSymbol* _globalScope;

	std::string _temporaryPath;
	std::string _objectPath;
	std::string _outputPath;
	std::string _debugPath;
	std::string _linkerPath;
	CompileFormat _outputFormat;
	bool _outputGUI;

	std::ofstream _outputFile;

public:
	CodeGenerator();
	
	bool Generate(std::string outputPath, CompileFormat format, bool isGUI, FunctionSymbol* globalScope);
	int GenerateASMNames(Symbol* symbol, int counter);
	
	void Write(std::string val);
	void WriteLine(std::string val);
	void WriteHeader();
	void WriteSymbolHeader(Symbol* symbol, bool isExtern);

	void WriteCodeSection();
	void WriteSymbolCode(Symbol* symbol);
	void WriteDataSection();
	void WriteMetaDataSection();
	void WriteSymbolData(Symbol* symbol);
	void WriteMetaData(Symbol* symbol);
	void WriteInstruction(Instruction* instr);

	int OptimizeInstructions(Symbol* symbol);
	int PeepHoleOptimizePushPop(Symbol* symbol);
	int PeepHoleOptimizePopPush(Symbol* symbol);

	int CountInstructions(Symbol* symbol);

	void EmitFunctionMetaData(Symbol* symbol);
	void EmitVariableMetaData(Symbol* symbol);

	std::string GetFunctionMetaDataFirst(Symbol* symbol);
	std::string GetVariableMetaDataFirst(Symbol* symbol);

	std::string FindStringLiteralLabel(std::string lit, bool nullTerminated=false);

};