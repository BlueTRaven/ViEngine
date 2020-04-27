#pragma once

#include "ViProgram.h"
#include "ViEnvironment.h"

namespace vigame
{
	class ProgramUnlitGeneric : public ViProgram
	{
	public:
		ProgramUnlitGeneric();

		void SetUniforms(ViVertexBatchInstance& aInstance) override;

		void SetCamera(mat4 aCamera);

		void SetTintColor(vec3 aTintColor);

	private:
		ViUniformMat4* mUniformProjection;
		ViUniformMat4* mUniformCamera;
		ViUniformVec3* mUniformTintColor;
	};
}