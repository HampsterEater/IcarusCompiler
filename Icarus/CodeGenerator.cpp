// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

// --------------------------------------------------------------------------------
//  Header Includes
// --------------------------------------------------------------------------------
#include "stdafx.h"

#define EMIT_ASM_CODE
#define OPTIMIZE_ASM

// --------------------------------------------------------------------------------
//	Initializes the code generator.
// --------------------------------------------------------------------------------
CodeGenerator::CodeGenerator()
{

}

// --------------------------------------------------------------------------------
//	Makes the generator take the instruction tree and converts
//  to an executable file.
// --------------------------------------------------------------------------------
bool CodeGenerator::Generate(std::string outputPath, CompileFormat format, bool isGUI, FunctionSymbol* globalScope)
{
	std::string basePath = StringHelper::ExtractDir(StringHelper::ExtractDir(g_app->GetExecutablePath()));
	std::string libPath = basePath + "\\lib";
	
	_outputPath = outputPath;
	_outputFormat = format;
	_outputGUI = isGUI;
	_temporaryPath = outputPath + ".s";
	_debugPath = outputPath + ".d";
	_objectPath = outputPath + ".o";
	_linkerPath = basePath + "\\tmp\\ld.tmp";
	_globalScope = globalScope;
	
	// --------------------------------------------------------------------------------
	//  Clean up old files.
	// --------------------------------------------------------------------------------
#ifdef EMIT_ASM_CODE
	FileHelper::DeleteFile(_temporaryPath);
#endif
	FileHelper::DeleteFile(_debugPath);
	FileHelper::DeleteFile(_objectPath);
	FileHelper::DeleteFile(_linkerPath);
	FileHelper::DeleteFile(outputPath);

	// --------------------------------------------------------------------------------
	//	Go through all the symbols and symbol instructions
	//  and generate unique asm names for each one.
	// --------------------------------------------------------------------------------
	GenerateASMNames(globalScope, 0);
	globalScope->UniqueASMName = "__icr_main";

	// --------------------------------------------------------------------------------
	//	Optimize Instructions.
	// --------------------------------------------------------------------------------
#ifdef OPTIMIZE_ASM

	printf(" - Optimizing instruction tree ...\n");
	int totalCount = CountInstructions(globalScope);
	int optimizeCount = 0;
	while (true)
	{
		int count = OptimizeInstructions(globalScope);
		if (count <= 0) break;
		optimizeCount += 1;
	}
	printf(" - Optimized away %i instructions of %i (%f%%).\n", optimizeCount, totalCount, ((float)optimizeCount / (float)totalCount) * 100.0f);

#endif

	// --------------------------------------------------------------------------------
	//	Output Assembly
	// --------------------------------------------------------------------------------
	
#ifdef EMIT_ASM_CODE
	// Open the assembly file.
	_outputFile.open(_temporaryPath, std::ios_base::out|std::ios_base::trunc);
	if (_outputFile.is_open() == false)
		return false;

	// Write the header.
	WriteHeader();
	
	// Write the code segment.
	WriteCodeSection();

	// Write the data segment.
	WriteDataSection();

	// Write the meta-data segment.
	WriteMetaDataSection();

	// Close the assembly file.
    _outputFile.close();
#endif

	// --------------------------------------------------------------------------------
	//	Assemble Object Files
	// --------------------------------------------------------------------------------

	// Compile the assembly file into our final output.
	printf(" - Assembling object files ...\n\n");
	std::string arguments = StringHelper::StripDir(_temporaryPath) + " " + StringHelper::StripDir(_objectPath) + " -s " + StringHelper::StripDir(_debugPath) + " -m 307200 -p 65536";
	if (!FileHelper::Execute("fasm.exe", arguments, StringHelper::ExtractDir(_temporaryPath)))
		g_app->EmitError("An internal error occured. Could not execute assembler.", ES_ASSEMBLER, EL_FATAL, 0, 0, _temporaryPath);
	printf("\n");

	// Check output file exists.
	if (!FileHelper::FileExists(_objectPath))
		g_app->EmitError("An internal error occured. FASM did not correctly build assembly code.", ES_ASSEMBLER, EL_FATAL, 0, 0, _temporaryPath);
	
	// --------------------------------------------------------------------------------
	//	Create Linker Object File
	// --------------------------------------------------------------------------------

	// Lets create our linker object file.
	std::ofstream linkerFile;
	linkerFile.open(_linkerPath, std::ios_base::out|std::ios_base::trunc);
	if (linkerFile.is_open() == false)
		return false;

	// Emit the linker files.
	linkerFile << "INPUT(\n";
	linkerFile << "\"" + libPath + "\\crtbegin.o\"\n";

	if (format == CF_LIBRARY)
		linkerFile << "\"" + libPath + "\\dllcrt2.o\"\n";
	else
		linkerFile << "\"" + libPath + "\\crt2.o\"\n";
	
	linkerFile << "\"" + _objectPath + "\"\n";
	linkerFile << "\"" + libPath + "\\AppStub_MW.a\"\n";

	// Add all the default linker libraries to the list.
	_linkLibraries.clear();
	if (format == CF_LIBRARY)
		_linkLibraries.push_back("crtdll");
	
	_linkLibraries.push_back("shell32");
	_linkLibraries.push_back("comctl32");
	_linkLibraries.push_back("comdlg32");
	_linkLibraries.push_back("glu32");
	_linkLibraries.push_back("opengl32");
	_linkLibraries.push_back("ole32");
	_linkLibraries.push_back("oleaut32");
	_linkLibraries.push_back("uuid");
	_linkLibraries.push_back("msimg32");

	_linkLibraries.push_back("gdi32");
	_linkLibraries.push_back("wsock32");
	_linkLibraries.push_back("winmm");
	_linkLibraries.push_back("advapi32");
	if (format != CF_LIBRARY)
		_linkLibraries.push_back("stdc++");
	else
		_linkLibraries.push_back("stdc++.dll");
	_linkLibraries.push_back("mingwex");
	_linkLibraries.push_back("mingw32");
	_linkLibraries.push_back("gcc_s");
	_linkLibraries.push_back("moldname");
	_linkLibraries.push_back("msvcrt");
	_linkLibraries.push_back("user32");
	_linkLibraries.push_back("kernel32");

	for (int i = 0; i < (int)_linkLibraries.size(); i++)
		linkerFile << "-l" + _linkLibraries[i] + "\n";

	linkerFile << "\"" + libPath + "\\crtend.o\"\n";
	linkerFile << ")";

	// Close linker file.
	linkerFile.close();
	
	// --------------------------------------------------------------------------------
	//	Link Object Files
	// --------------------------------------------------------------------------------

	// Link stub and main object file together into executable.
	printf(" - Linking object files ...\n\n");
	arguments = "-s --enable-auto-import -stack 4194304 "+std::string(format == CF_LIBRARY ? "-shared" : "")+" -subsystem " + std::string(isGUI == true ? "windows" : "console") + " -o \"" + (outputPath) + "\" -L\"" + libPath + "\" \"" + _linkerPath + "\"";
	if (!FileHelper::Execute("ld.exe", arguments, ""))
		g_app->EmitError("An internal error occured. Could not execute linker.", ES_LINKER, EL_FATAL, 0, 0, _temporaryPath);

	// Check output file exists.
	if (!FileHelper::FileExists(outputPath))
	{	
		printf("\n");
		g_app->EmitError("An internal error occured. LD did not correctly link object files.", ES_LINKER, EL_FATAL, 0, 0, _temporaryPath);
	}

	return true;
}	

// --------------------------------------------------------------------------------
//	Returns the assembly label for a given string literal.
// --------------------------------------------------------------------------------
std::string CodeGenerator::FindStringLiteralLabel(std::string lit, bool nullTerminated)
{
	Symbol* symbol = _globalScope->FindString(lit, nullTerminated);
	if (symbol == NULL)
		g_app->EmitError("An internal error occured. Code generator could not find label for string literal '"+lit+"'.", ES_LINKER, EL_FATAL, 0, 0, _temporaryPath);
	
	return symbol->UniqueASMName;
}

// --------------------------------------------------------------------------------
//	This function will recursively generate unique assembly level names for all 
//  instructions and symbols used in references.
// --------------------------------------------------------------------------------
int CodeGenerator::GenerateASMNames(Symbol* symbol, int counter)
{
	// If its a internal compiler function/variable then we need a name
	// that can't conflict with any others.
	if (symbol->GeneratedASMName == false)
	{
		if (symbol->GetSymbolType() == ST_FUNCTION)
		{
			if (((FunctionSymbol*)symbol)->IsExtern == true)
				symbol->UniqueASMName = "_icr_" + symbol->Identifier;
			else
			{
				std::string scope = "";
				Symbol* parent = symbol->Scope;
				while (parent != NULL && parent != _globalScope)
				{
					scope = parent->Identifier + "_" + scope;
					parent = parent->Scope;
				}		
				
				symbol->UniqueASMName = "___" + scope + symbol->Identifier+"_" + StringHelper::IntToString(((FunctionSymbol*)symbol)->ParameterCount);	
			}
		}
		else
		{
			if (symbol->GetSymbolType() == ST_STRING)
			{
				symbol->UniqueASMName = "___string_" + StringHelper::IntToString(counter++); 
			}
			else
			{
				if (symbol->Identifier.substr(0, 2) == "$_")
					symbol->UniqueASMName = "__icr_c_" + symbol->Identifier.substr(2) + "_" + StringHelper::IntToString(counter++);
				else		
					symbol->UniqueASMName = "___" + symbol->Identifier + "_" + StringHelper::IntToString(counter++);
			}
		}
		symbol->GeneratedASMName = true;
	}

	for (int i = 0; i < (int)symbol->Instructions.size(); i++)
		symbol->Instructions.at(i)->UniqueASMName = "___instr" + StringHelper::IntToString(counter++); 

	for (int i = 0; i < (int)symbol->Children.size(); i++)
		counter = GenerateASMNames(symbol->Children.at(i), counter);

	return counter;
}

// --------------------------------------------------------------------------------
//	These two functions just write output to the temporary assembly file.
// --------------------------------------------------------------------------------
void CodeGenerator::Write(std::string val)
{
	_outputFile << val;
}
void CodeGenerator::WriteLine(std::string val)
{
	_outputFile << val;
	_outputFile << "\n";
}

// --------------------------------------------------------------------------------
//	Writes the header of the assembly code.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteHeader()
{
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	;  Icarus Assembly Listing");
	WriteLine("	;  " + _outputPath);
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	format MS COFF");
	WriteLine("");
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	;  Import external functions.");
	WriteLine("	; ---------------------------------------------------");
	WriteSymbolHeader(_globalScope, true);
	WriteLine("");
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	;  Make our functions externally accessible.");
	WriteLine("	; ---------------------------------------------------");
	WriteSymbolHeader(_globalScope, false);
	WriteLine("");
}

// --------------------------------------------------------------------------------
//	Writes the header data out for a given symbol.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteSymbolHeader(Symbol* symbol, bool isExtern)
{
	if ((symbol->GeneratedSymbolHeaderExtern == false && isExtern == true) ||
		(symbol->GeneratedSymbolHeader == false && isExtern == false))
	{
		if (isExtern == true)
			symbol->GeneratedSymbolHeaderExtern = true;
		else
			symbol->GeneratedSymbolHeader = true;

		switch (symbol->GetSymbolType())
		{
			// Emit data for a function.
			case ST_FUNCTION:
			{
				if (((FunctionSymbol*)symbol)->IsExtern == isExtern)
					WriteLine("	" + std::string(((FunctionSymbol*)symbol)->IsExtern == true ? "extrn" : "public") + " " + symbol->UniqueASMName);

				break;
			}

			// Emit the definition code for a class.
			case ST_CLASS:
			{
				// No header data.
				break;
			}

			// Emit data for a string.
			case ST_STRING:
			{
				// No header data.
				break;
			}

			// Emit data for a variable.
			case ST_VARIABLE:
			{
				// No data
				break;
			}
		}
	}

	// Emit all sub symbols.
	for (int i = 0; i < (int)symbol->Children.size(); i++)
		WriteSymbolHeader(symbol->Children.at(i), isExtern);
}

// --------------------------------------------------------------------------------
//	Writes out the code section.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteCodeSection()
{
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	;  Code Section");
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	section '.text' code readable executable");
	WriteLine("");

	WriteSymbolCode(_globalScope);
}

// --------------------------------------------------------------------------------
//	Writes the code out for a given symbol.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteSymbolCode(Symbol* symbol)
{
	if (symbol->GeneratedSymbolCode == false)
	{
		symbol->GeneratedSymbolCode = true;

		switch (symbol->GetSymbolType())
		{
			// Emit code for a function.
			case ST_FUNCTION:
			{
				if (((FunctionSymbol*)symbol)->IsExtern == false)
				{
					WriteLine(symbol->UniqueASMName + ":");

					for (int i = 0; i < (int)symbol->Instructions.size(); i++)
						WriteInstruction(symbol->Instructions.at(i));

					WriteLine("");
				}
				break;
			}

			// Emit the definition code for a class.
			case ST_CLASS:
			{
				// No code.
				break;
			}
		
			// Emit data for a string.
			case ST_STRING:
			{
				// No code.
				break;
			}
		
			// Emit code for a variable.
			case ST_VARIABLE:
			{
				// Variable symbols have no code.
				break;
			}
		}
	}

	// Emit all sub symbols.
	for (int i = 0; i < (int)symbol->Children.size(); i++)
		WriteSymbolCode(symbol->Children.at(i));
}

// --------------------------------------------------------------------------------
//	Writes out the assembly definition of an instruction.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteInstruction(Instruction* instr)
{
	switch (instr->OpCode)
	{
		case OPCODE_JUMP_TARGET:
		{
			WriteLine(instr->UniqueASMName + ":");
			break;
		}

		case OPCODE_COMMENT:
		{
			std::string s = instr->Operands[0]->CreateAssembly();
			if (s != "")
				WriteLine("\n\t; " + instr->Operands[0]->CreateAssembly());
			break;
		}

		// Emit as a normal instruction.
		default:
		{
			std::string line = "\t";
			line += OpCodeMnemonics[(int)instr->OpCode] + " ";

			for (int i = 0; i < instr->OpCount; i++)
			{
				line += instr->Operands[i]->CreateAssembly();
				if (i < instr->OpCount - 1)
					line += ", ";
			}

			WriteLine(line);
		}
	}
}

// --------------------------------------------------------------------------------
//	Writes out the data section.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteDataSection()
{
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	;  Data Section");
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	section '.data' data writeable");
	WriteLine("");

	WriteSymbolData(_globalScope);
}

// --------------------------------------------------------------------------------
//	Writes out the meta-data section.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteMetaDataSection()
{	
	WriteLine("");
	WriteLine("	; ---------------------------------------------------");
	WriteLine("	;  Metadata Section");
	WriteLine("	; ---------------------------------------------------");
	WriteLine("");

	WriteMetaData(_globalScope);
}

// --------------------------------------------------------------------------------
//	Writes the data out for a given symbol.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteMetaData(Symbol* symbol)
{
	if (symbol->GeneratedSymbolData == false)
	{
		symbol->GeneratedSymbolData = true;
		
		// Write out the header.
		WriteLine(symbol->UniqueASMName + "_meta:");
	
			// General information.
			switch (symbol->GetSymbolType())
			{
				case ST_VARIABLE:	WriteLine("\tdd " + StringHelper::IntToString(META_TYPE_VARIABLE)); break;
				case ST_FUNCTION:	WriteLine("\tdd " + StringHelper::IntToString(META_TYPE_FUNCTION)); break;
				case ST_CLASS:		WriteLine("\tdd " + StringHelper::IntToString(META_TYPE_CLASS)); break;
				case ST_NAMESPACE:	WriteLine("\tdd " + StringHelper::IntToString(META_TYPE_NAMESPACE)); break;
			}

			// Source code declaration.
			WriteLine("\tdd " + FindStringLiteralLabel(symbol->DefinitionToken.Path, true));
			WriteLine("\tdd " + StringHelper::IntToString(symbol->DefinitionToken.Line));
			WriteLine("\tdd " + StringHelper::IntToString(symbol->DefinitionToken.Column));
		
			// General properties.
			WriteLine("\tdd " + FindStringLiteralLabel(symbol->Identifier, true));
		
			// Child properties.
			WriteLine("\tdd " + symbol->UniqueASMName + "_meta_children");

			// Write in specific symbol information.
			switch (symbol->GetSymbolType())
			{
				// Emit a namespace!
				case ST_NAMESPACE:
				{
					NamespaceSymbol* func = (NamespaceSymbol*)symbol;
					break;
				}

				// Emit metadata for a function.
				case ST_FUNCTION:
				{
					FunctionSymbol* func = (FunctionSymbol*)symbol;
					
					WriteLine("\tdd " + std::string(func->IsExtern == false ? func->UniqueASMName : "0"));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->ReturnDataType->GetPrimitive()));
					WriteLine("\tdd " + StringHelper::IntToString(func->ReturnDataType->GetPrimitive() == PDT_CLASS ? ((ClassDataType*)func->ReturnDataType)->Class->ClassDefinitionIndex : 0));
					WriteLine("\tdd " + StringHelper::IntToString(func->ParameterCount));
					WriteLine("\tdd " + StringHelper::IntToString(func->ParameterSize));
					WriteLine("\tdd " + StringHelper::IntToString(func->LocalCount));
					WriteLine("\tdd " + StringHelper::IntToString(func->LocalSize));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->IsExtern));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->IsMethod));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->IsGenerator));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->IsVirtual));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->IsBase));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->VirtualFunctionTableOffset));
					WriteLine("\tdd " + StringHelper::IntToString((int)func->AccessModifier));

					break;
				}
			
				// Emit metadata for a variable.
				case ST_VARIABLE:
				{
					VariableSymbol* var = (VariableSymbol*)symbol;
					
					WriteLine("\tdd " + std::string(var->Scope == _globalScope ? var->UniqueASMName : "0"));
					WriteLine("\tdd " + StringHelper::IntToString((int)var->VariableType));
					WriteLine("\tdd " + StringHelper::IntToString((int)var->DataType->GetPrimitive()));
					WriteLine("\tdd " + StringHelper::IntToString(var->DataType->GetPrimitive() == PDT_CLASS ? ((ClassDataType*)var->DataType)->Class->ClassDefinitionIndex : 0));
					WriteLine("\tdd " + StringHelper::IntToString(var->StackOffset));
					WriteLine("\tdd " + StringHelper::IntToString(var->ParamStackOffset));
					WriteLine("\tdd " + StringHelper::IntToString(var->ClassOffset));
					WriteLine("\tdd " + StringHelper::IntToString((int)var->IsConst));
					WriteLine("\tdd " + StringHelper::IntToString((int)var->IsBase));			
					WriteLine("\tdd " + StringHelper::IntToString((int)var->AccessModifier));

					break;
				}
			
				// Emit metadata for a class.
				case ST_CLASS:
				{
					ClassSymbol* classSymbol = (ClassSymbol*)symbol;

					WriteLine("\tdd " + classSymbol->UniqueASMName + "_vftable");
					WriteLine("\tdd " + StringHelper::IntToString(classSymbol->VirtualFunctionTableSize));
				
					WriteLine("\tdd " + StringHelper::IntToString(classSymbol->MemoryDataSize));
					
					if (classSymbol->SuperClass != NULL)
						WriteLine("\tdd " + StringHelper::IntToString(classSymbol->SuperClass->ClassDefinitionIndex));
					else
						WriteLine("\tdd -1");
					
					WriteLine("\tdd " + StringHelper::IntToString(classSymbol->ClassDefinitionIndex));

					WriteLine("\tdd " + (classSymbol->InternalConstructMethod == NULL ? "0" : classSymbol->InternalConstructMethod->UniqueASMName));
					WriteLine("\tdd " + (classSymbol->InternalDisposeMethod == NULL ? "0" : classSymbol->InternalDisposeMethod->UniqueASMName));
					
					// Write out the virtual-table.
					WriteLine(classSymbol->UniqueASMName + "_vftable:");
					for (int i = 0; i < classSymbol->VirtualFunctionTableSize; i++)
					{
						// Emit dat virtual functions in order.
						for (int j = 0; j < classSymbol->Children.size(); j++)
						{
							FunctionSymbol* func = dynamic_cast<FunctionSymbol*>(classSymbol->Children.at(j));
							if (func != NULL && func->VirtualFunctionTableOffset == i)
							{
								//printf("%i = %s\n", i, func->Identifier.c_str());
								WriteLine("\tdd " + func->UniqueASMName);
							}
						}
					}

					break;
				}
			}

		WriteLine("");

		// Define the list of children.
		WriteLine(symbol->UniqueASMName + "_meta_children:");
		for (int i = 0; i < (int)symbol->Children.size(); i++)
		{
			Symbol* child = symbol->Children.at(i);
			if (child->GetSymbolType() == ST_CLASS || 
				child->GetSymbolType() == ST_VARIABLE || 
				child->GetSymbolType() == ST_FUNCTION || 
				child->GetSymbolType() == ST_NAMESPACE)
				WriteLine("\tdd " + child->UniqueASMName + "_meta");
		}	
		WriteLine("\tdd 0");
		WriteLine("");

		// Emit the children!
		for (int i = 0; i < (int)symbol->Children.size(); i++)
		{
			Symbol* child = symbol->Children.at(i);
			if (child->GetSymbolType() == ST_CLASS || 
				child->GetSymbolType() == ST_VARIABLE || 
				child->GetSymbolType() == ST_FUNCTION || 
				child->GetSymbolType() == ST_NAMESPACE)
				WriteMetaData(child);
		}	
	}

	// Emit all sub symbols.
//	for (int i = 0; i < (int)symbol->Children.size(); i++)
//		WriteMetaData(symbol->Children.at(i));
}

// --------------------------------------------------------------------------------
//	Writes the data out for a given symbol.
// --------------------------------------------------------------------------------
void CodeGenerator::WriteSymbolData(Symbol* symbol)
{
//	if (symbol->GeneratedSymbolData == false)
//	{
//		symbol->GeneratedSymbolData = true;

		switch (symbol->GetSymbolType())
		{
			// Emit data for a function.
			case ST_FUNCTION:
			{
				// Emit global scope information!
				//if (symbol == _globalScope)
				//{
				//	WriteLine(symbol->UniqueASMName + "_meta:");
					
				//	WriteLine("\tdd " + GetFunctionMetaDataFirst(symbol));
				//	WriteLine("\tdd " + GetVariableMetaDataFirst(symbol));

				//	EmitFunctionMetaData(symbol);
				//	EmitVariableMetaData(symbol);
				//}

				break;
			}

			// Emit data for a string.
			case ST_STRING:
			{
				if (((StringSymbol*)symbol)->IsNullTerminated == true)
				{
					WriteLine(symbol->UniqueASMName + ":");
					WriteLine("\tdb \"" + symbol->Identifier + "\",0");
					WriteLine("");
				}
				else
				{
					WriteLine(symbol->UniqueASMName + ":");
				
					// Get string class.	
					ClassSymbol* classSymbol = (ClassSymbol*)_globalScope->FindChild("string", false, true);

					// Object header.
					//WriteLine("\tdd " + StringHelper::IntToString(classSymbol->ClassDefinitionIndex)); // Class Definition Index
					//WriteLine("\tdd 0");

					// Length
					WriteLine("\tdd " + StringHelper::IntToString(symbol->Identifier.size())); 

					// Data Pointer
					WriteLine("\tdd " + symbol->UniqueASMName+"_data"); 
					WriteLine("");

					WriteLine(symbol->UniqueASMName + "_data:");

					if (symbol->Identifier.size() != 0)
					{
						Write("\tdb "); 
						for (int i = 0; i < symbol->Identifier.size(); i++)
						{
							char chr = symbol->Identifier.at(i);
							Write(StringHelper::IntToString((int)chr));

							if (i < symbol->Identifier.size() - 1)
								Write(",");
						}
						Write("\n");
					}

					WriteLine("");
				}

				break;
			}
		
			// Emit the definition code for a class.
			/*
			case ST_CLASS:
			{
				ClassSymbol* classSymbol = (ClassSymbol*)symbol;
			
				// Write out the definition code.
				WriteLine(classSymbol->UniqueASMName + ":");
				WriteLine("\tdd " + classSymbol->UniqueASMName + "_vftable");
				WriteLine("\tdd " + StringHelper::IntToString(classSymbol->VirtualFunctionTableSize));

				WriteLine("\tdd " + FindStringLiteralLabel(classSymbol->DefinitionToken.Path, true));
				WriteLine("\tdd " + StringHelper::IntToString(classSymbol->DefinitionToken.Line));
				WriteLine("\tdd " + StringHelper::IntToString(classSymbol->DefinitionToken.Column));
				WriteLine("\tdd " + FindStringLiteralLabel(classSymbol->Identifier, true));
				WriteLine("\tdd " + StringHelper::IntToString(classSymbol->MemoryDataSize));

				if (classSymbol->SuperClass != NULL)
					WriteLine("\tdd " + StringHelper::IntToString(classSymbol->SuperClass->ClassDefinitionIndex));
				else
					WriteLine("\tdd -1");

				WriteLine("\tdd " + (classSymbol->InternalConstructMethod == NULL ? "0" : classSymbol->InternalConstructMethod->UniqueASMName));
				WriteLine("\tdd " + (classSymbol->InternalDisposeMethod == NULL ? "0" : classSymbol->InternalDisposeMethod->UniqueASMName));

			//	WriteLine("\tdd " + GetFunctionMetaDataFirst(classSymbol));
			//	WriteLine("\tdd " + GetVariableMetaDataFirst(classSymbol));

			//	EmitFunctionMetaData(classSymbol);
		//		EmitVariableMetaData(classSymbol);

				// Write out the virtual-table.
				//printf("\nClass: %s\n", classSymbol->Identifier.c_str());
				WriteLine(classSymbol->UniqueASMName + "_vftable:");
				for (int i = 0; i < classSymbol->VirtualFunctionTableSize; i++)
				{
					// Emit dat virtual functions in order.
					for (int j = 0; j < classSymbol->Children.size(); j++)
					{
						FunctionSymbol* func = dynamic_cast<FunctionSymbol*>(classSymbol->Children.at(j));
						if (func != NULL && func->VirtualFunctionTableOffset == i)
						{
							//printf("%i = %s\n", i, func->Identifier.c_str());
							WriteLine("\tdd " + func->UniqueASMName);
						}
					}
				}

				WriteLine("");

				break;
			}*/

			// Emit data for a variable.
			case ST_VARIABLE:
			{
				VariableSymbol* varSymbol = (VariableSymbol*)symbol;
				switch (varSymbol->VariableType)
				{
					case VT_CONST:
					case VT_GLOBAL:
						{
							std::string defType = varSymbol->DataType->GetAssemblyDefName();
							WriteLine("align 4");
							WriteLine(varSymbol->UniqueASMName + ":");
							WriteLine("\t" + defType + " 0");
							WriteLine("");
							// TODO CHANGE BACK

							// Global Data 
							break;
						}

					default:
						{
							// No data
							break;
						}
				}

				break;
			}
		}
//	}

	// Emit all sub symbols.
	for (int i = 0; i < (int)symbol->Children.size(); i++)
		WriteSymbolData(symbol->Children.at(i));
}

// --------------------------------------------------------------------------------
//	Peep hole optimization, awwwwh yeeeeh.
// --------------------------------------------------------------------------------
int CodeGenerator::OptimizeInstructions(Symbol* symbol)
{
	int count = 0;

	// Go through all instructions and see if we can optimize.
	//count += PeepHoleOptimizePushPop(symbol); 
	//count += PeepHoleOptimizePopPush(symbol); BROKEN - Messes up on . access.

	// Optimize Sub-Symbols.
	for (int i = 0; i < (int)symbol->Children.size(); i++)
		count += OptimizeInstructions(symbol->Children.at(i));

	return count;
}

// --------------------------------------------------------------------------------
//	Tries to replace unneccessary push/pop pairs.
// --------------------------------------------------------------------------------
int CodeGenerator::PeepHoleOptimizePushPop(Symbol* symbol)
{
	std::vector<Instruction*> _pushStack;
	std::vector<Instruction*> _popStack;
	bool inPush = false;

	for (int i = 0; i < (int)symbol->Instructions.size(); i++)
	{
		Instruction* instruction = symbol->Instructions.at(i);

		// Check for push/pops in secession.
		bool optimize = true;
		if (instruction->OpCode == OPCODE_PUSH || instruction->OpCode == OPCODE_PUSHD)
		{
			_pushStack.push_back(instruction);
			optimize = false;
		}
		else if (instruction->OpCode == OPCODE_POP)
		{
			if (_pushStack.size() == 0)
			{
				_pushStack.clear();
				_popStack.clear();
				optimize = false;
			}
			else
			{
				_popStack.push_back(instruction);
				optimize = (_pushStack.size() == _popStack.size()); // Push directly followed by pop
			}
		}
		
		if (optimize == true)
		{
			// Can we optimize the instructions?
			if (_pushStack.size() > 0 && _popStack.size() > 0 && 
				_pushStack.size() >= _popStack.size())
			{
				// If there are more pushs than pops, remove the first few.
				while (_pushStack.size() > _popStack.size())
				{
					_pushStack.erase(_pushStack.begin());
				}

				// Check they are sequential. Pushs before the pops.
				int minPopIndex = -1;
				for (int j = 0; j < _popStack.size(); j++)
				{
					int index = symbol->GetInstructionIndex(_popStack.at(j));
					if (index < minPopIndex || minPopIndex == -1)
						minPopIndex = index;
				}
				int maxPushIndex = -1;
				for (int j = 0; j < _pushStack.size(); j++)
				{
					int index = symbol->GetInstructionIndex(_pushStack.at(j));
					if (index < maxPushIndex || maxPushIndex == -1)
						maxPushIndex = index;
				}

				bool sequential = true;
				for (int j = 0; j < _pushStack.size(); j++)
				{
					// All push isntructions should be before the first pop instruction.
					int index = symbol->GetInstructionIndex(_pushStack.at(j));
					if (index >= minPopIndex)
					{
						sequential = false;
						break;
					}

					// All pop instructions should be after the last push instruction.
					index = symbol->GetInstructionIndex(_popStack.at(j));
					if (index <= maxPushIndex)
					{
						sequential = false;
						break;
					}
				}

				// Go through each push/pop pair and convert push into a mov.
				int removedInstructions = 0;
				for (int i = 0; i < _pushStack.size(); i++)
				{
					Instruction* pushInstruction = _pushStack.at(i);
					Instruction* popInstruction = _popStack.at(_popStack.size() - (i + 1));
				
					int popIndex = symbol->GetInstructionIndex(popInstruction);
					int pushIndex = symbol->GetInstructionIndex(pushInstruction);

					// Turn push into mov.
					pushInstruction->OpCode = OPCODE_MOV;
					pushInstruction->Operands[1] = pushInstruction->Operands[0]; // src  = push op
					pushInstruction->Operands[0] = popInstruction->Operands[0];  // dest = pop op
					pushInstruction->OpCount = 2;

					// Remove the pop instruction.
					symbol->Instructions.erase(symbol->Instructions.begin() + popIndex);
					removedInstructions++;
				}

				return removedInstructions; // Just do them one at a time.
			}

			_pushStack.clear();
			_popStack.clear();
		}
	}

	return 0;
}

// --------------------------------------------------------------------------------
//	Removes redundent stack access like this;
//		pop reg1
//		push reg1
// --------------------------------------------------------------------------------
int CodeGenerator::PeepHoleOptimizePopPush(Symbol* symbol)
{
	for (int i = 0; i < (int)symbol->Instructions.size() - 1; i++)
	{
		Instruction* popInstruction = symbol->Instructions.at(i);
		Instruction* pushInstruction = symbol->Instructions.at(i + 1);
		
		int popIndex = symbol->GetInstructionIndex(popInstruction);

		if ((popInstruction->OpCode == OPCODE_POP) &&
			(pushInstruction->OpCode == OPCODE_PUSH || pushInstruction->OpCode == OPCODE_PUSHD))
		{
			// Check its same register.
			Operand* op1 = popInstruction->Operands[0];
			Operand* op2 = pushInstruction->Operands[0];
			if (op1->IsEqualTo(op2) == true)
			{
				symbol->Instructions.erase(symbol->Instructions.begin() + popIndex); // Remove POP
				symbol->Instructions.erase(symbol->Instructions.begin() + popIndex); // Remove PUSH
				return 1;
			}
		}
	}

	return 0;
}

// --------------------------------------------------------------------------------
//	Counts how many instructions the symbol and its sub-symbols contain.
// --------------------------------------------------------------------------------
int CodeGenerator::CountInstructions(Symbol* symbol)
{
	int count = 0;

	for (int i = 0; i < (int)symbol->Instructions.size(); i++)
	{
		Instruction* instr = symbol->Instructions.at(i);
		if (instr->OpCode != OPCODE_COMMENT &&
			instr->OpCode != OPCODE_JUMP_TARGET)
			count++;
	}

	// Optimize Sub-Symbols.
	for (int i = 0; i < (int)symbol->Children.size(); i++)
		count += CountInstructions(symbol->Children.at(i));

	return count;
}