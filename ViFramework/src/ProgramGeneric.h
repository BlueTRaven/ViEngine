#pragma once

#include <glm/glm.hpp>

#include "ViColor.h"
#include "ViProgram.h"
#include "ViUtil.h"
#include "ViEnvironment.h"

class VIENGINE_EXPORT ViProgramGeneric : public ViProgram
{
public:
	ViProgramGeneric();

	void SetUniforms() override;

	void SetProjection(mat4 aProjection);
	void SetCamera(mat4 aCamera);
	void SetColor(ViColorGL aColor);

	vi_property_get_named(mat4, mProjection, Projection);
	vi_property_get_named(mat4, mCamera, Camera);

	vi_property_get_named(ViColorGL, mColor, Color);

private:
	mat4 mProjection;
	mat4 mCamera;

	ViColorGL mColor;
};