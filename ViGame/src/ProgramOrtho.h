#pragma once

#include "ViProgram.h"
#include "ViEnvironment.h"
#include "ViUniformMat4.h"
#include "ViUniformVec4.h"

namespace vigame
{
	class ProgramOrtho : public ViProgram
	{
	public:
		ProgramOrtho();

		void SetUniforms(ViVertexBatchInstance& aInstance) override;

		void SetProjection(mat4 aProjection);
		void SetTintColor(ViColorGL aColor);

	private:
		ViUniformMat4* mUniformProjection;
		ViUniformMat4* mUniformCamera;
		ViUniformVec3* mUniformTintColor;
	};
}