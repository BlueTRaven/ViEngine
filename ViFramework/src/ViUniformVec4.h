#pragma once

#include "glm/glm.hpp"

#include "ViUniform.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViUniformVec4 : public ViUniform<vec4>
{
public:
	ViUniformVec4(std::string aName, vec4 aDefault);

	void Upload(ViProgram* aProgram) override;
};