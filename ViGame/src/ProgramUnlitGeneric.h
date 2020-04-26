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

	private:
		ViUniformMat4* mUniformProjection;
		ViUniformMat4* mUniformCamera;

	};
}