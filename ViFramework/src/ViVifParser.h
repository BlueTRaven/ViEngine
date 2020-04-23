#pragma once

#include <fstream>
#include <vector>
#include <string>

#include "ViVifLine.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViVifParser
{
public:
	ViVifParser();
	ViVifParser(std::string path);

	void Parse(std::string path);

	std::vector<ViVifLine> GetLines();
	
	//Finds a ViVifLine with the first word given as aKeyName.
	ViVifLine FindLine(string aKeyName);

	//Finds a ViVifLine with aKeyName as the first word. It then interprets and sets aValue to this.
	//If this word does not exist, or it could not be properly interpreted, aValue will not be modified.
	void FindVec3i(string aKeyName, vec3i& aValue);
	//Finds a ViVifLine with aKeyName as the first word. It then interprets and sets aValue to this.
	//If this word does not exist, or it could not be properly interpreted, aValue will not be modified.
	void FindVec3(string aKeyName, vec3& aValue);
	//Finds a ViVifLine with aKeyName as the first word. It then interprets and sets aValue to this.
	//If this word does not exist, or it could not be properly interpreted, aValue will not be modified.
	void FindString(string aKeyName, string& aValue);
	//Finds a ViVifLine with aKeyName as the first word. It then interprets and sets aValue to this.
	//If this word does not exist, or it could not be properly interpreted, aValue will not be modified.
	void FindFloat(string aKeyName, float& aValue);
	//Finds a ViVifLine with aKeyName as the first word. It then interprets and sets aValue to this.
	//If this word does not exist, or it could not be properly interpreted, aValue will not be modified.
	void FindInt(string aKeyName, int& aValue);

	vi_property_named(bool, mValid, Valid);

private:
	DISABLE_WARNING_PUSH
	DISABLE_WARNING(4251)
	std::vector<ViVifLine> mLines;
	DISABLE_WARNING_POP
};