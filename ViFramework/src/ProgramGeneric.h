#pragma once

#include <glm/glm.hpp>

#include "ViColor.h"
#include "ViProgram.h"
#include "ViUtil.h"
#include "ViUniformVec3.h"
#include "ViUniformVec4.h"
#include "ViUniformFloat.h"
#include "ViColorsGL.h"

class VIENGINE_EXPORT ViProgramGeneric : public ViProgram
{
public:
	ViProgramGeneric();

	void SetUniforms(ViVertexBatchInstance& aInstance) override;

	void SetProjection(mat4 aProjection);
	void SetCamera(mat4 aCamera);
	void SetTintColor(ViColorGL aColor);

	void SetAmbientColor(ViColorGL aColor);
	void SetAmbientStrength(float aStrength);

	void SetDiffusePos(vec3 aPos);

	void InterpretParams(std::vector<ViVifLine> aParams) override;

private:
	ViUniformMat4* mUniformProjection;
	ViUniformMat4* mUniformCamera;
	ViUniformVec4* mUniformTintColor;

	ViUniformVec3* mUniformDiffuseColor;
	ViUniformVec3* mUniformDiffusePos;
	ViUniformFloat* mUniformDiffuseStrength;

	ViUniformVec3* mUniformAmbientColor;
	ViUniformFloat* mUniformAmbientStrength;

	ViUniformMat4* mUniformNormalObject;

	bool mDiffuseEnabled = true;
	bool mAmbientEnabled = true;
};