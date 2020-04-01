#include "ProgramGeneric.h"

#include "ViEnvironment.h"

ViProgramGeneric::ViProgramGeneric() :
	ViProgram(viEnv->GetAssetHandler()->LoadShader("vertex_generic"),
		viEnv->GetAssetHandler()->LoadShader("fragment_generic"),
		std::vector<ViVertexAttribute> {
	ViVertexAttribute("normal", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, nrm)),
		ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
		ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
		ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
}),
mUniformProjection(new ViUniformMat4("projection", glm::perspectiveFov(45.0, double(viEnv->GetScreenWidth()), double(viEnv->GetScreenHeight()), 0.01, 1000.0))),
mUniformCamera(new ViUniformMat4("camera", glm::identity<glm::mat4>())),
mUniformTintColor(new ViUniformVec4("tint_color", vicolors::WHITE.ToVec4())),
mUniformDiffuseColor(new ViUniformVec3("diffuse_color", vicolors::WHITE.ToVec4())),
mUniformDiffusePos(new ViUniformVec3("diffuse_pos", { 0.0, 1.0, -1.0 })),
mUniformDiffuseStrength(new ViUniformFloat("diffuse_strength", 1.f)),
mUniformAmbientColor(new ViUniformVec3("ambient_color", vicolors::WHITE.ToVec4())),
mUniformAmbientStrength(new ViUniformFloat("ambient_strength", 1.f)),
mUniformNormalObject(new ViUniformMat4("normal_object", glm::identity<mat4>()))
{
}

void ViProgramGeneric::SetUniforms(ViVertexBatchInstance& aInstance)
{
	if (!GetDirty())
		return;
	
	mUniformProjection->Upload(this);
	mUniformCamera->Upload(this);
	mUniformTintColor->Upload(this);

	mUniformDiffuseColor->Upload(this);
	mUniformDiffusePos->Upload(this);
	mUniformDiffuseStrength->Upload(this);
	
	mUniformAmbientColor->Upload(this);
	mUniformAmbientStrength->Upload(this);

	mUniformNormalObject->Upload(this);

	ViProgram::SetUniforms(aInstance);
}

void ViProgramGeneric::SetProjection(glm::mat4 aProjection)
{
	SetDirty(true);
	mUniformProjection->Set(this, aProjection);
}

void ViProgramGeneric::SetCamera(glm::mat4 aCamera)
{
	SetDirty(true);
	mUniformCamera->Set(this, aCamera);
}

void ViProgramGeneric::SetTintColor(ViColorGL aColor)
{
	SetDirty(true);
	mUniformTintColor->Set(this, aColor.ToVec4());
}

void ViProgramGeneric::SetAmbientColor(ViColorGL aColor)
{
	SetDirty(true);
	mUniformAmbientColor->Set(this, aColor.ToVec4());
}

void ViProgramGeneric::SetAmbientStrength(float aStrength)
{
	SetDirty(true);
	mUniformAmbientStrength->Set(this, aStrength);
}

void ViProgramGeneric::SetDiffusePos(vec3 aPos)
{
	SetDirty(true);
	mUniformDiffusePos->Set(this, aPos);
}

void ViProgramGeneric::InterpretParams(std::vector<ViVifLine> aParams)
{
	for (ViVifLine line : aParams)
	{
		if (line.mWords[1] == "diffuse")
		{
			mDiffuseEnabled = std::stoi(line.mWords[2]);
			mUniformDiffuseStrength->Set(this, mDiffuseEnabled);
		}
		else if (line.mWords[1] == "ambient")
		{
			mAmbientEnabled = std::stoi(line.mWords[2]);
			mUniformAmbientStrength->Set(this, mAmbientEnabled);
		}
	}
}
