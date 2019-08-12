#include "StringHelpers.h"
#include "algorithm"
#include <cctype>

void SStringHelpers::Ltrim(std::string &S)
{
	S.erase(S.begin(), std::find_if(S.begin(), S.end(), [](const int Ch) {
		return !std::isspace(Ch);
	}));
}

void SStringHelpers::Rtrim(std::string &S)
{
	S.erase(std::find_if(S.rbegin(), S.rend(), [](const int Ch) {
		return !std::isspace(Ch);
	}).base(), S.end());
}

void SStringHelpers::Trim(std::string& S)
{
	Ltrim(S);
	Rtrim(S);
}

void SStringHelpers::FindAndReplaceAll(std::string& Data, const std::string& ToSearch, const std::string& ReplaceStr)
{
	size_t Pos = Data.find(ToSearch);

	while (Pos != std::string::npos)
	{
		Data.replace(Pos, ToSearch.size(), ReplaceStr);
		Pos = Data.find(ToSearch, Pos + ReplaceStr.size());
	}
}

void SStringHelpers::ToUpper(std::string& InString)
{
	std::transform(InString.begin(), InString.end(), InString.begin(),
		[](const unsigned char C) -> unsigned char { return std::toupper(C); });
}

std::string SStringHelpers::ToUpper(const std::string& InString)
{
	std::string ReturnStr;
	std::transform(InString.begin(), InString.end(), ReturnStr.begin(),
		[](const unsigned char C) -> unsigned char { return std::toupper(C); });
	return ReturnStr;
}

std::vector<std::string> SStringHelpers::SplitString(const std::string& InString, const std::string& Delimiter)
{
	std::string Str = InString;
	std::vector<std::string> Words;
	size_t Pos = 0;

	while ((Pos = Str.find(Delimiter)) != std::string::npos)
	{
		if (Str.substr(0, Pos).empty())
		{
			Str.erase(0, Pos + Delimiter.length());
			continue;
		}
		if (Str.substr(0, Pos).empty())
		{
			Str.erase(0, Pos);
			continue;
		}
		Words.push_back(Str.substr(0, Pos));
		Str.erase(0, Pos + Delimiter.length());
	}

	Words.push_back(Str);
	Words.erase(
		std::remove_if(
			Words.begin(),
			Words.end(),
			[](std::string const& S) { return S.empty(); }),
		Words.end());

	return Words;
}

std::string SStringHelpers::LetterToPhonetic(const std::string& Letter)
{
	if (Letter == "A") return "ALFA";
	if (Letter == "B") return "BRAVO";
	if (Letter == "C") return "CHARLIE";
	if (Letter == "D") return "DELTA";
	if (Letter == "E") return "ECHO";
	if (Letter == "F") return "FOXTROT";
	if (Letter == "G") return "GOLF";
	if (Letter == "H") return "HOTEL";
	if (Letter == "I") return "INDIA";
	if (Letter == "J") return "JULIET";
	if (Letter == "K") return "KILO";
	if (Letter == "L") return "LIMA";
	if (Letter == "M") return "MIKE";
	if (Letter == "N") return "NOVEMBER";
	if (Letter == "O") return "OSCAR";
	if (Letter == "P") return "PAPA";
	if (Letter == "Q") return "QUEBEC";
	if (Letter == "R") return "ROMEO";
	if (Letter == "S") return "SIERRA";
	if (Letter == "T") return "TANGO";
	if (Letter == "U") return "UNIFORM";
	if (Letter == "V") return "VICTOR";
	if (Letter == "W") return "WHISKEY";
	if (Letter == "X") return "X-RAY";
	if (Letter == "Y") return "YANKEE";
	if (Letter == "Z") return "ZULU";

	return "NA";
}