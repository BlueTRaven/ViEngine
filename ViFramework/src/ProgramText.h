#pragma once

#include <glm/glm.hpp>

#include "ViColorsGL.h"
#include "ViColor.h"
#include "ViVertex.h"
#include "ViProgram.h"
#include "ViUtil.h"
#include "ViUniformMat4.h"
#include "ViUniformVec4.h"

class VIENGINE_EXPORT ViProgramText : public ViProgram
{
public:
	ViProgramText();

	void SetUniforms(ViVertexBatchInstance& aInstance) override;

	void SetProjection(mat4 aProjection);
	void SetColor(ViColorGL aColor);

private:
	ViUniformMat4* mUniformProjection;
	ViUniformVec4* mUniformTintColor;
};