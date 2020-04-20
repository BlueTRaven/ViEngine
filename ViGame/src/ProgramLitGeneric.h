#pragma once

#include "ViProgram.h"
#include "ViUniformVec3.h"

namespace vigame
{
	class ProgramLitGeneric : public ViProgram
	{
	public:
		ProgramLitGeneric();

		void SetUniforms(ViVertexBatchInstance& aInstance) override;

		void SetLightPosition(vec3 aPosition);

		void SetCamera(mat4 aCamera);

	private:
		ViUniformMat4* mUniformProjection;
		ViUniformMat4* mUniformCamera;

		ViUniformVec3* mUniformLightPos;
		ViUniformVec3* mUniformLightColor;
	};
}