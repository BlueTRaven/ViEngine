#pragma once

#include "ViProgram.h"
#include "ViEnvironment.h"

namespace vigame
{
	class ProgramShadowmap : public ViProgram
	{
	public:
		ProgramShadowmap();

		void SetUniforms(ViVertexBatchInstance& aInstance) override;

		void SetProjection(mat4 aProjection);
		void SetCamera(mat4 aCamera);

	private:
		ViUniformMat4* mUniformProjection;
		ViUniformMat4* mUniformCamera;
	};
}