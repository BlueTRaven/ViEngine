#pragma once

#include <vector>

#include "ViUtil.h"

struct VIENGINE_EXPORT ViVifLine
{
	ViVifLine();
	ViVifLine(std::vector<std::string> line);

	DISABLE_WARNING_PUSH
	DISABLE_WARNING(4251)
	std::vector<std::string> mWords;
	DISABLE_WARNING_POP

	bool mIsEmpty;
};