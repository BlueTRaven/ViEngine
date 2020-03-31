#pragma once

#include "glm/glm.hpp"

#include "ViUniform.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViUniformMat4 : public ViUniform<mat4>
{
public:
	ViUniformMat4(std::string aName, mat4 aDefault);

	void Upload(ViProgram* aProgram) override;
};