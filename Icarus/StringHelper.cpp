// --------------------------------------------------------------------------------
//	Icarus Programming Language
//  Copyright © 2010 Timothy Leonard
// --------------------------------------------------------------------------------

#include "stdafx.h"

// --------------------------------------------------------------------------------
//	Splits a string multiple times and returns the results.
//
//   input     : Input string to split.
//   seperator : Character to split string by.
//   maxSplits : Maximum number of times to split string.
//   output    : Pointer to string array to place results in.
//
//   Returns   : Number of times string was split.
// --------------------------------------------------------------------------------
int StringHelper::Split(std::string input, char seperator, int maxSplits, std::string* output, bool ignoreEmpty)
{
	size_t startIndex;
	size_t charIndex;
	std::string subSplit;
	int splitIndex;

	// Check that we can actual split the string with the memory allocated.
	if (maxSplits <= 1)
		return -1;

	// Check there is at least one splitter character.
	charIndex = input.find_first_of(seperator);
	if (charIndex < 0)
	{
		output[0] = input;
		return 1;
	}

	// Keep looking for splits.
	charIndex = 0;
	startIndex = 0;
	splitIndex = 0;
	while (charIndex != std::string::npos)
	{
		// Work out the position of the next seperator.
		charIndex = input.find_first_of(seperator, startIndex);

		// Extract the next segment of the string.
		subSplit = input.substr(startIndex, (charIndex - startIndex));
		startIndex = charIndex + 1;

		// Make sure it's not an empty string if we are ignoring them.
		if (subSplit != "" || ignoreEmpty == false)
		{
			output[splitIndex++] = subSplit;

			// If we only have one slot left, put the last of string in it.
			if (splitIndex >= maxSplits - 1)
			{
				output[splitIndex++] = input.substr(startIndex);
				break;
			}
		}
	}

	return splitIndex;
}

// --------------------------------------------------------------------------------
//	Converts a string to a long integer.
//
//   input     : String to convert.
//
//   Returns   : Long equivilent of input.
// --------------------------------------------------------------------------------
signed long long StringHelper::StringToLong(std::string val)
{
	return _atoi64(val.c_str());
}

// --------------------------------------------------------------------------------
//	Converts a string to a integer.
//
//   input     : String to convert.
//
//   Returns   : Integer equivilent of input.
// --------------------------------------------------------------------------------
signed int StringHelper::StringToInt(std::string val)
{
	return atoi(val.c_str());
}

// --------------------------------------------------------------------------------
//	Converts an integer to an std::string.
//
//   input     : Integer to convert.
//
//   Returns   : String equivilent of input.
// --------------------------------------------------------------------------------
std::string StringHelper::IntToString(int input)
{
	char convertArray[256];
	sprintf_s(convertArray, 256, "%i", input);
	return std::string(convertArray);
}

// --------------------------------------------------------------------------------
//	Converts a string to upper case.
//
//   val       : String to convert to uppercase.
//
//   Returns   : Uppercase equivilent of string.
// --------------------------------------------------------------------------------
std::string StringHelper::ToUpper(std::string val)
{
	std::transform(val.begin(), val.end(), val.begin(), ::toupper);
	return val;
}

// --------------------------------------------------------------------------------
//	Converts a string to lower case.
//
//   val       : String to convert to lowercase.
//
//   Returns   : Lowercase equivilent of string.
// --------------------------------------------------------------------------------
std::string StringHelper::ToLower(std::string val)
{
	std::transform(val.begin(), val.end(), val.begin(), ::tolower);
	return val;
}

// --------------------------------------------------------------------------------
//  Returns the directory segment of a path.
// --------------------------------------------------------------------------------
std::string StringHelper::ExtractDir(std::string val)
{
	// Find the last index of the / or \ characters.
	int index = -1;
	for (int i = 0; i < (int)val.length(); i++)
	{
		if (val.at(i) == '\\' || val.at(i) == '/')
			index = i;
	}
	if (index == -1)
		return val;
	
	// Return the directory segment.
	return val.substr(0, index);
}

// --------------------------------------------------------------------------------
//  Returns the file-name segment of a path.
// --------------------------------------------------------------------------------
std::string StringHelper::StripDir(std::string val)
{
	// Find the last index of the / or \ characters.
	int index = -1;
	for (int i = 0; i < (int)val.length(); i++)
	{
		if (val.at(i) == '\\' || val.at(i) == '/')
			index = i;
	}
	if (index == -1)
		return val;
	
	// Return the file name segment.
	return val.substr(index + 1);
}
