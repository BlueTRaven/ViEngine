#pragma once

#include "ViProgram.h"
#include "ViUniformVec3.h"
#include "ViUniformFloat.h"

namespace vigame
{
	class ProgramLitGeneric : public ViProgram
	{
	public:
		struct DiffuseLight
		{
			vec3 position;
			vec3 color;
			float strength;
		};

		struct RadialFog
		{
			vec3 color;
			float start;
			float end;
		};

		ProgramLitGeneric();

		void SetUniforms(ViVertexBatchInstance& aInstance) override;

		void SetDiffuseLight(DiffuseLight aLight);

		void SetRadialFog(RadialFog aFog);

		void SetCamera(mat4 aCamera);

		void SetTintColor(vec3 aTintColor);

		void SetObjectMat(mat4 aMatrix) override;

	private:
		ViUniformMat4* mUniformProjection;
		ViUniformMat4* mUniformCamera;
		ViUniformMat4* mUniformObjectNormal;
		ViUniformVec3* mUniformTintColor;

		ViUniformVec3* mUniformLightPos;
		ViUniformVec3* mUniformLightColor;
		ViUniformFloat* mUniformLightStrength;

		ViUniformVec3* mUniformRadialFogColor;
		ViUniformFloat* mUniformRadialFogStart;
		ViUniformFloat* mUniformRadialFogEnd;
	};
}