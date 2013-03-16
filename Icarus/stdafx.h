// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#define APP_VERSION "1.0.0.0"

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <algorithm>
#include <Windows.h>

#include "Icarus.h"
#include "Globals.h"

#include "FileHelper.h"
#include "StringHelper.h"

#include "IcarusCompilerApp.h"
#include "LexicalAnalyser.h"

#include "DataType.h"
#include "QWordDataType.h"
#include "BoolDataType.h"
#include "ByteDataType.h"
#include "ClassDataType.h"
#include "DoubleDataType.h"
#include "FloatDataType.h"
#include "IntDataType.h"
#include "LongDataType.h"
#include "ShortDataType.h"
#include "StringDataType.h"
#include "VoidDataType.h"

#include "Operand.h"
#include "Instruction.h"

#include "Symbol.h"
#include "FunctionSymbol.h"
#include "VariableSymbol.h"
#include "ClassSymbol.h"
#include "StringSymbol.h"
#include "NamespaceSymbol.h"

#include "PreProcessor.h"
#include "Parser.h"
#include "CodeGenerator.h"
