#pragma once

#include <unordered_map>
#include <unordered_set>

#include "ViEnvironment.h"
#include "ViProgram.h"
#include "ViUtil.h"
#include "ViColorsGL.h"
#include "ViUniform.h"
#include "ViUniformMat4.h"
#include "ViUniformVec3.h"
#include "ViUniformVec4.h"
#include "ViUniformFloat.h"

#include "ViGameUtil.h"

namespace vigame
{
	class ProgramCubesInstanced : public ViProgram
	{
	public:
		ProgramCubesInstanced();

		//void Bind(ViVertexBatchInstance& aInstance, ViVertexBatchSettings aSettings) override;

		void SetUniforms(ViVertexBatchInstance& aInstance) override;

		void SetProjection(mat4 aProjection);
		void SetCamera(mat4 aCamera);
		void SetTintColor(ViColorGL aColor);

		void SetAmbientColor(ViColorGL aColor);
		void SetAmbientStrength(float aStrength);

		void SetDiffusePos(vec3 aPos);

		//void SetObjectMat(mat4 aObjMat) override;

		/*vi_property_get_named(mat4, mProjection, Projection);
		vi_property_get_named(mat4, mCamera, Camera);
		vi_property_get_named(ViColorGL, mTintColor, TintColor);

		vi_property_get_named(float, mAmbientStrength, AmbientStrength);
		vi_property_get_named(ViColorGL, mAmbientColor, AmbientColor);

		vi_property_get_named(vec3, mDiffusePos, DiffuseLightPos);*/

		void BindAttributes(ViVertexBatchInstance& aInstance, bool aForceBind = false) override;

		void SetInstance(vec3i aPosition, cubeid aId, cubeid aOldId);

	private:
		ViVertexAttribute mInstancePosAttribute;

		ViUniformMat4* mUniformProjection;
		ViUniformMat4* mUniformCamera;
		ViUniformVec4* mUniformTintColor;

		ViUniformVec3* mUniformDiffuseColor;
		ViUniformVec3* mUniformDiffusePos;

		ViUniformVec3* mUniformAmbientColor;
		ViUniformFloat* mUniformAmbientStrength;

		ViUniformMat4* mUniformNormalObject;

		bool mInstancesDirty;	//instances require reupload
		std::unordered_map<cubeid, std::unordered_set<vec3i>> mInstances;

		std::unordered_map<cubeid, GLuint> mInstancesBuffers;

		std::unordered_set<cubeid> mDirtyIds;

		std::vector<vec3i> GetPositions(cubeid aId);
	};
}