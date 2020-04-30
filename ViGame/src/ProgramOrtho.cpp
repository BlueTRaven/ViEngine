#include "ProgramOrtho.h"

vigame::ProgramOrtho::ProgramOrtho() :
	ViProgram(ENVIRONMENT->GetAssetHandler()->LoadShader("vertex_lit_generic"),
		ENVIRONMENT->GetAssetHandler()->LoadShader("fragment_unlit_generic"),
		std::vector<ViVertexAttribute> {
		ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
		ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
		ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
}),
mUniformProjection(new ViUniformMat4("projection", glm::ortho(0.0, double(ENVIRONMENT->GetScreenWidth()), 0.0, double(ENVIRONMENT->GetScreenHeight()), -1.0, 1.0))),
mUniformCamera(new ViUniformMat4("camera", glm::identity<mat4>())),
mUniformTintColor(new ViUniformVec3("tint_color", vicolors::WHITE.ToVec4()))
{
	SetDirty(true);
}

void vigame::ProgramOrtho::SetUniforms(ViVertexBatchInstance & aInstance)
{
	mUniformProjection->Upload(this);
	mUniformCamera->Upload(this);
	mUniformTintColor->Upload(this);

	ViProgram::SetUniforms(aInstance);
}

void vigame::ProgramOrtho::SetProjection(glm::mat4 aProjection)
{
	SetDirty(true);
	mUniformProjection->Set(this, aProjection);
}

void vigame::ProgramOrtho::SetTintColor(ViColorGL aColor)
{
	SetDirty(true);
	mUniformTintColor->Set(this, aColor.ToVec4());
}