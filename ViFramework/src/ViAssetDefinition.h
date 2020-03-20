#pragma once

#include <string>
#include <vector>

#include "ViVifLine.h"

struct ViAssetDefinition
{
	std::string name;
	std::string type;
	std::vector<ViVifLine> definition;
	bool hasFound;
};