#include "ProgramCubesInstanced.h"

vigame::ProgramCubesInstanced::ProgramCubesInstanced() : ViProgram(ENVIRONMENT->GetAssetHandler()->LoadShader("vertex_cubesinstanced"),
	ENVIRONMENT->GetAssetHandler()->LoadShader("fragment_generic"),
	std::vector<ViVertexAttribute> {
	ViVertexAttribute("normal", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, nrm)),
		ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
		ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
		ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord)),
}),
mUniformProjection(new ViUniformMat4("projection", glm::perspectiveFov(glm::radians(90.0), double(ENVIRONMENT->GetScreenWidth()), double(ENVIRONMENT->GetScreenHeight()), 0.01, 1000.0))),
mUniformCamera(new ViUniformMat4("camera", glm::identity<glm::mat4>())),
mUniformTintColor(new ViUniformVec4("tint_color", vicolors::WHITE.ToVec4())),
mUniformDiffuseColor(new ViUniformVec3("diffuse_color", vicolors::WHITE.ToVec4())),
mUniformDiffusePos(new ViUniformVec3("diffuse_pos", { 0.0, 1.0, -1.0 })),
mUniformAmbientColor(new ViUniformVec3("ambient_color", vicolors::WHITE.ToVec4())),
mUniformAmbientStrength(new ViUniformFloat("ambient_strength", 1.f)),
mInstancePosAttribute(ViVertexAttribute("instance_pos", 3, 0, nullptr))
{
}

/*void vigame::ProgramCubesInstanced::Bind(ViVertexBatchInstance & aInstance, ViVertexBatchSettings aSettings)
{
	cubeid id = (cubeid)aInstance.info;

	aInstance.instanced = true;
	aInstance.instancedCount = mInstances[id].size();

	if (mDirtyIds.find(id) != mDirtyIds.end())
	{
		auto positions = GetPositions(id);

		if (mInstancesBuffers.find(id) == mInstancesBuffers.end())
		{
			GLuint buffer = 0;
			glGenBuffers(1, &buffer);

			mInstancesBuffers.emplace(id, buffer);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mInstancesBuffers[id]);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3i), positions.data(), GL_STATIC_DRAW);
	}

	GLuint loc = glGetAttribLocation(GetId(), "instance_pos");
	glEnableVertexAttribArray(loc);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancesBuffers[id]);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	//BindAttribute(mInstancePosAttribute);
	glVertexAttribDivisor(mInstancePosAttribute.Get_id(), 1);

	mDirtyIds.erase(id);
}*/

void vigame::ProgramCubesInstanced::SetUniforms(ViVertexBatchInstance& aInstance)
{
	if (!GetDirty())
		return;

	mUniformProjection->Upload(this);
	mUniformCamera->Upload(this);
	mUniformTintColor->Upload(this);

	mUniformDiffuseColor->Upload(this);
	mUniformDiffusePos->Upload(this);

	mUniformAmbientColor->Upload(this);
	mUniformAmbientStrength->Upload(this);

	mUniformNormalObject->Upload(this);
}

void vigame::ProgramCubesInstanced::BindAttributes(ViVertexBatchInstance& aInstance, bool aForceBind)
{
	//TODO remove this class
	cubeid id = (cubeid)aInstance.info;

	if (mDirtyIds.find(id) != mDirtyIds.end())
	{
		auto positions = GetPositions(id);

		if (mInstancesBuffers.find(id) == mInstancesBuffers.end())
		{
			GLuint buffer = 0;
			glGenBuffers(1, &buffer);

			mInstancesBuffers.emplace(id, buffer);
		}

		glBindBuffer(GL_ARRAY_BUFFER, mInstancesBuffers[id]);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3i), positions.data(), GL_STATIC_DRAW);
	}

	GLuint loc = glGetAttribLocation(GetId(), "instance_pos");
	glEnableVertexAttribArray(loc);
	glBindBuffer(GL_ARRAY_BUFFER, mInstancesBuffers[id]);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
		//BindAttribute(mInstancePosAttribute);
	glVertexAttribDivisor(mInstancePosAttribute.Get_id(), 1);

	mDirtyIds.erase(id);

	aInstance.mesh->Bind();

	ViProgram::BindAttributes(aInstance, aForceBind);
}

void vigame::ProgramCubesInstanced::SetProjection(glm::mat4 aProjection)
{
	SetDirty(true);
	mUniformProjection->Set(this, aProjection);
	//mProjection = aProjection;
}

void vigame::ProgramCubesInstanced::SetCamera(glm::mat4 aCamera)
{
	SetDirty(true);
	mUniformCamera->Set(this, aCamera);
	//mCamera = aCamera;
}

void vigame::ProgramCubesInstanced::SetTintColor(ViColorGL aColor)
{
	SetDirty(true);
	mUniformTintColor->Set(this, aColor.ToVec4());
	//mTintColor = aColor;
}

void vigame::ProgramCubesInstanced::SetAmbientColor(ViColorGL aColor)
{
	SetDirty(true);
	mUniformAmbientColor->Set(this, aColor.ToVec4());
	//mAmbientColor = aColor;
}

void vigame::ProgramCubesInstanced::SetAmbientStrength(float aStrength)
{
	SetDirty(true);
	mUniformAmbientStrength->Set(this, aStrength);
	//mAmbientStrength = aStrength;
}

void vigame::ProgramCubesInstanced::SetDiffusePos(vec3 aPos)
{
	SetDirty(true);
	mUniformDiffusePos->Set(this, aPos);
	//mDiffusePos = aPos;
}

void vigame::ProgramCubesInstanced::SetInstance(vec3i aPosition, cubeid aId, cubeid aOldId)
{
	//Don't need to update if ids are the same
	if (aId == aOldId)
		return;

	//first, remove old
	//Don't bother trying to erase the instance if it's id 0
	if (aOldId != 0 && mInstances.find(aOldId) != mInstances.end())
	{
		auto& set = mInstances[aOldId];

		//if we find the element, remove it
		//Note that we should never *not* find the element... if that happens, aOldId is incorrect
		if (set.find(aPosition) != set.end())
			set.erase(aPosition);
	}

	//second, add to new
	//note that if aId == 0, we don't bother adding it
	//(there's no point in keeping track of id 0 since that's always going to be an empty cube with no model, and thus never needs to be drawn.)
	if (aId != 0 && mInstances.find(aId) != mInstances.end())
	{
		auto& set = mInstances[aId];

		//If we don't find the position, put it in there
		if (set.find(aPosition) == set.end())
		{
			set.emplace(aPosition);
			
			if (mDirtyIds.find(aId) == mDirtyIds.end())
				mDirtyIds.emplace(aId);
		}
	}
	else mInstances.emplace(aId, std::unordered_set<vec3i> { aPosition });
	//else mInstances.emplace(aPosition);
}

std::vector<vec3i> vigame::ProgramCubesInstanced::GetPositions(cubeid aId)
{
	auto set = mInstances[aId];

	std::vector<vec3i> out(set.begin(), set.end());

	return out;
}
