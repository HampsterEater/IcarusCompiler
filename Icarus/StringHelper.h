// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	This class contains several methods devoted to helping manipulate strings.
// --------------------------------------------------------------------------------
class StringHelper
{
private:

public:
	static int Split(std::string input, char seperator, int maxSplits, std::string* output, bool ignoreEmpty = false);
	static std::string IntToString(int val);
	static long long StringToLong(std::string val);
	static int StringToInt(std::string val);
	static std::string ToUpper(std::string val);
	static std::string ToLower(std::string val);

	static std::string ExtractDir(std::string val);
	static std::string StripDir(std::string val);

};