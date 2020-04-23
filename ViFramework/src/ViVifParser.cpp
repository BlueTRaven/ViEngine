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

void ViVifParser::FindVec3i(std::string aKeyName, vec3i& aDefault)
{
	if (!GetValid())
		return;

	ViVifLine line = FindLine(aKeyName);

	if (line.mIsEmpty)
		return;

	if (line.mWords.size() != 4)	//four words: name, x, y, z.
		return;

	aDefault = vec3i(std::stoi(line.mWords[1]), std::stoi(line.mWords[2]), std::stoi(line.mWords[3]));
}

void ViVifParser::FindVec3(std::string aKeyName, vec3& aDefault)
{
	if (!GetValid())
		return;

	ViVifLine line = FindLine(aKeyName);

	if (line.mIsEmpty)
		return;

	if (line.mWords.size() != 4)	//four words: name, x, y, z.
		return;

	aDefault = vec3(std::stof(line.mWords[1]), std::stof(line.mWords[2]), std::stof(line.mWords[3]));
}

void ViVifParser::FindString(std::string aKeyName, string& aValue)
{
	if (!GetValid())
		return;

	ViVifLine line = FindLine(aKeyName);

	if (line.mIsEmpty)
		return;

	if (line.mWords.size() != 2)	//two words: name, value.
		return;

	aValue = line.mWords[1];
}

void ViVifParser::FindFloat(std::string aKeyName, float& aValue)
{
	if (!GetValid())
		return;

	ViVifLine line = FindLine(aKeyName);

	if (line.mIsEmpty)
		return;

	if (line.mWords.size() != 2)	//two words: name, value.
		return;

	aValue = std::stof(line.mWords[1]);
}

void ViVifParser::FindInt(std::string aKeyName, int& aValue)
{
	if (!GetValid())
		return;

	ViVifLine line = FindLine(aKeyName);

	if (line.mIsEmpty)
		return;

	if (line.mWords.size() != 2)	//two words: name, value.
		return;

	aValue = std::stoi(line.mWords[1]);
}

std::vector<ViVifLine> ViVifParser::GetLines()
{
	return mLines;
}
