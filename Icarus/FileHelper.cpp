// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	Checks if a file exists.
//
//   path    : Path of file to check if it exists.
//
//   Returns : Returns true if file exists, otherwise false.
// --------------------------------------------------------------------------------
bool FileHelper::FileExists(std::string path)
{
	std::ifstream ifile(path);
	return ifile != NULL ? true : false;
}

// --------------------------------------------------------------------------------
//	Deletes the given file.
//
//   path    : Path of file to delete.
//
//   Returns : Returns true if file was deleted, otherwise false.
// --------------------------------------------------------------------------------
bool FileHelper::DeleteFile(std::string path)
{
	return remove(path.c_str()) == 0 ? true : false;
}

// --------------------------------------------------------------------------------
//	Loads all the text out of a file.
//
//   path    : Path of file to load text from.
//
//   Returns : Returns the string contents of the given file.
// --------------------------------------------------------------------------------
std::string FileHelper::LoadText(std::string path)
{
	std::ifstream file;
	if (file == NULL)
		return "";

	file.open(path, std::ios_base::in);
	if (file.is_open() == false)
		return "";

	std::string contents;
    while (file.good())
	{
		char chr = (char)file.get();
		if (!file.good())
			break;

		contents += chr;
	}

    file.close();

	return contents;
}

// --------------------------------------------------------------------------------
//	Saves the given string into a file.
//
//   path    : Path of file to load text from.
//   text    : Text to save into file.
//
//   Returns : True if successful, otherwise false.
// --------------------------------------------------------------------------------
bool FileHelper::SaveText(std::string path, std::string contents)
{
	std::ofstream file;
	if (file == NULL)
		return false;

	file.open(path, std::ios_base::out|std::ios_base::trunc);
	if (file.is_open() == false)
		return false;

    file << contents;

    file.close();

	return true;
}

// --------------------------------------------------------------------------------
//	Executes the given executable with the given arguments.
//
//   path      : Path to executable to run.
//   arguments : Arguments to pass to executable.
//   directory : Working directory.
//
//   Returns : True if successful, otherwise false.
// --------------------------------------------------------------------------------
bool FileHelper::Execute(std::string path, std::string arguments, std::string directory)
{
    STARTUPINFOA       siStartupInfo;
    PROCESS_INFORMATION piProcessInfo;

    memset(&siStartupInfo, 0, sizeof(siStartupInfo));
    memset(&piProcessInfo, 0, sizeof(piProcessInfo));

    siStartupInfo.cb = sizeof(siStartupInfo);

	std::string finalArg = std::string("Shell " + arguments);
	char argArray[256];
	char* argArrayPtr = (char*)(&argArray);

	if (finalArg.size() < 256)
	{
		finalArg.copy(argArrayPtr, finalArg.size());
		argArray[finalArg.size()] = '\0';
	}
    int res = CreateProcessA(path.c_str(),  
                    (char*)(&argArray),                 
                     0,
                     0,
                     FALSE,
                     CREATE_DEFAULT_ERROR_MODE,
                     0,
                     directory == "" ? 0 : directory.c_str(),                            
                     &siStartupInfo,
                     &piProcessInfo);
	
	if (res == 0)
		return false;

	WaitForSingleObject(piProcessInfo.hProcess, INFINITE);

	return true;
	
	/*
	HINSTANCE res = ShellExecuteA(NULL, "open", path.c_str(), arguments.c_str(), NULL, SW_SHOWNORMAL);

	if (reinterpret_cast<int>(res) > 32)
		return true;
	
	return false;
	*/
}