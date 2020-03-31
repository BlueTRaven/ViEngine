#pragma once

#include <string>

#include "ViProgram.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViAssetHolderProgramFactory
{
public:
	virtual ViProgram* Create(std::string type);
};