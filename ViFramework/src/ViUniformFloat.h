#pragma once

#include "glm/glm.hpp"

#include "ViUniform.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViUniformFloat : public ViUniform<float>
{
public:
	ViUniformFloat(std::string aName, float aDefault);

	void Upload(ViProgram* aProgram) override;
};