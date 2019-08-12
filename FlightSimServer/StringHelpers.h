// /*
//  * (C)20192019 KaosSpectrum
//  * 
//  * Released under GPL v3 Licence.
//  *
//  * Do not remove this copyright!

#pragma once
#include "string"
#include "vector"

struct SStringHelpers
{
public:
	// trim from start (in place)
	static void Ltrim(std::string& S);

	// trim from end (in place)
	static void Rtrim(std::string& S);

	// trim from both ends (in place)
	static void Trim(std::string& S);

	//Find and replace all (in-place)
	static void FindAndReplaceAll(std::string& Data, const std::string& ToSearch, const std::string& ReplaceStr);

	//String to uppercase
	static void ToUpper(std::string& InString);

	static std::string ToUpper(const std::string& InString);

	//split a string at delimiter
	static std::vector<std::string> SplitString(const std::string& InString, const std::string& Delimiter);

	//Converts letter to alphanumeric
	std::string LetterToPhonetic(const std::string& Letter);
};
