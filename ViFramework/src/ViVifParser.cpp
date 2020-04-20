#include "ViVifParser.h"

ViVifParser::ViVifParser()
{ }

ViVifParser::ViVifParser(std::string path)
{
	Parse(path);
}

void ViVifParser::Parse(std::string path)
{
	std::vector<std::string> lines;
	
	std::ifstream file(path);

	if (!file.is_open())
		return;

	std::string currstr;

	//first, put all different lines of the file into different strings in a vector.
	//This way, we can loop over them easier.
	while (std::getline(file, currstr))
		lines.push_back(currstr);

	for (std::string str : lines)
	{
		currstr = "";

		std::vector<std::string> outstrs;
		bool inliteral = false;

		for (char c : str)
		{
			if (c == '"')
			{
				inliteral = !inliteral;

				if (!inliteral)
				{
					outstrs.push_back(currstr);
					currstr = "";
				}

				continue;	//don't add to currstr
			}

			if (c == ' ' || c == '\n' || c == 0x0d || c == 0x0a || c == '#')
			{
				outstrs.push_back(currstr);
				currstr = "";
				continue;
			}

			currstr += c;
		}

		if (currstr.size() > 0)
			outstrs.push_back(currstr);

		mLines.push_back(ViVifLine(outstrs));
	}

	mValid = true;
}

ViVifLine ViVifParser::FindLine(std::string aKeyName)
{
	if (!GetValid())
		return ViVifLine();

	for (ViVifLine line : mLines)
	{
		if (!line.mIsEmpty && line.mWords[0] == aKeyName)
			return line;
	}

	return ViVifLine();
}

std::vector<ViVifLine> ViVifParser::GetLines()
{
	return mLines;
}
