#pragma once

#include <glm/glm.hpp>

#include "ViFont.h"

#include "stb/stb_truetype.h"

struct ViGlyphInfo
{
	ViGlyphInfo(ViFont* font, uint32_t character, float offX, float offY);

	float offX;
	float offY;

	glm::vec3 positions[4];
	glm::vec2 uvs[4];
};