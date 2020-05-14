#include "ProgramUnlitGeneric.h"

vigame::ProgramUnlitGeneric::ProgramUnlitGeneric() : ViProgram(ASSET_HANDLER->LoadShader("vertex_lit_generic"), ASSET_HANDLER->LoadShader("fragment_unlit_generic"), std::vector<ViVertexAttribute> {
	//ViVertexAttribute("normal", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, nrm)),
		ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
		//ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
		ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
}),
mUniformProjection(new ViUniformMat4("projection", glm::perspective(glm::radians(80.0), double(ENVIRONMENT->GetScreenWidth()) / double(ENVIRONMENT->GetScreenHeight()), 0.01, 1000.0))),
mUniformCamera(new ViUniformMat4("camera", glm::identity<glm::mat4>())),
mUniformTintColor(new ViUniformVec3("tint_color", vec3(1)))
{
}

void vigame::ProgramUnlitGeneric::SetUniforms(ViVertexBatchInstance & aInstance)
{
	mUniformProjection->Upload(this);
	mUniformCamera->Upload(this);
	mUniformTintColor->Upload(this);

	ViProgram::SetUniforms(aInstance);
}

void vigame::ProgramUnlitGeneric::SetCamera(mat4 aCamera)
{
	SetDirty(true);
	mUniformCamera->Set(this, aCamera);
}

void vigame::ProgramUnlitGeneric::SetTintColor(vec3 aTintColor)
{
	SetDirty(true);
	mUniformTintColor->Set(this, aTintColor);
}
