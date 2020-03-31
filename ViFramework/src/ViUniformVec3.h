#pragma once

#include "glm/glm.hpp"

#include "ViUniform.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViUniformVec3 : public ViUniform<vec3>
{
public:
	ViUniformVec3(std::string aName, vec3 aDefault);

	void Upload(ViProgram* aProgram) override;
};