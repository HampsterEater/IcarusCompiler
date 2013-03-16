// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------
#pragma once

#include "stdafx.h"

#include <fstream>

// --------------------------------------------------------------------------------
//	This class contains several methods devoted to helping manipulate files.
// --------------------------------------------------------------------------------
class FileHelper
{
private:

public:
	static bool FileExists(std::string path);
	static bool DeleteFile(std::string path);

	static std::string LoadText(std::string path);
	static bool SaveText(std::string path, std::string contents);

	static bool Execute(std::string path, std::string arguments, std::string directory);

};