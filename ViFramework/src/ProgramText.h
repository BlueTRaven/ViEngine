#pragma once

#include <glm/glm.hpp>

#include "ViColorsGL.h"
#include "ViColor.h"
#include "ViVertex.h"
#include "ViProgram.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViProgramText : public ViProgram
{
public:
	ViProgramText();

	void SetUniforms() override;

	void SetProjection(mat4 aProjection);
	void SetColor(ViColorGL aColor);

	vi_property_get_named(ViColorGL, mColor, Color);

private:
	mat4 mProjection;
	ViColorGL mColor;
};