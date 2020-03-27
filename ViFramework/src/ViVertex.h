#pragma once

#include <glm/glm.hpp>

#include "ViColor.h"

#include "ViUtil.h"

struct VIENGINE_EXPORT ViVertex
{
	ViVertex(vec3 nrm, vec3 pos, ViColorGL color, vec2 texCoord);

	vec3 nrm;
	vec3 pos;
	ViColorGL color;
	vec2 texCoord;
};