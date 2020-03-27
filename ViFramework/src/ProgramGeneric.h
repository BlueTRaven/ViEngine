#pragma once

#include <glm/glm.hpp>

#include "ViColor.h"
#include "ViProgram.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViProgramGeneric : public ViProgram
{
public:
	ViProgramGeneric();

	void SetUniforms() override;

	void SetProjection(mat4 aProjection);
	void SetCamera(mat4 aCamera);
	void SetTintColor(ViColorGL aColor);

	void SetAmbientColor(ViColorGL aColor);
	void SetAmbientStrength(float aStrength);

	void SetDiffusePos(vec3 aPos);

	void SetObjectMat(mat4 aObjMat) override;

	vi_property_get_named(mat4, mProjection, Projection);
	vi_property_get_named(mat4, mCamera, Camera);
	vi_property_get_named(ViColorGL, mTintColor, TintColor);

	vi_property_get_named(float, mAmbientStrength, AmbientStrength);
	vi_property_get_named(ViColorGL, mAmbientColor, AmbientColor);

	vi_property_get_named(vec3, mDiffusePos, DiffuseLightPos);

private:
	mat4 mProjection;
	mat4 mCamera;
	ViColorGL mTintColor;

	ViColorGL mDiffuseColor;
	vec3 mDiffusePos;

	ViColorGL mAmbientColor;
	float mAmbientStrength;

};