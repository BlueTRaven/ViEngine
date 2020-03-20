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
	
	vi_property_named(bool, mValid, Valid);

private:
	DISABLE_WARNING_PUSH
	DISABLE_WARNING(4251)
	std::vector<ViVifLine> mLines;
	DISABLE_WARNING_POP
};