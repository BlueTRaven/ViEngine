#include "ProgramShadowmap.h"

vigame::ProgramShadowmap::ProgramShadowmap() : 
	ViProgram(ASSET_HANDLER->LoadShader("vertex_lit_generic"), 
		ASSET_HANDLER->LoadShader("fragment_unlit_generic"), 
		std::vector<ViVertexAttribute> {
			//ViVertexAttribute("normal", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, nrm)),
			ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
			ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
			ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
		}),
mUniformProjection(new ViUniformMat4("projection", glm::perspective(glm::radians(80.0), double(ENVIRONMENT->GetScreenWidth()) / double(ENVIRONMENT->GetScreenHeight()), 0.01, 1000.0))),
mUniformCamera(new ViUniformMat4("camera", glm::identity<glm::mat4>()))
{
}

void vigame::ProgramShadowmap::SetUniforms(ViVertexBatchInstance & aInstance)
{
	mUniformProjection->Upload(this);
	mUniformCamera->Upload(this);

	ViProgram::SetUniforms(aInstance);
}

void vigame::ProgramShadowmap::SetProjection(mat4 aProjection)
{
	SetDirty(true);
	mUniformProjection->Set(this, aProjection);
}

void vigame::ProgramShadowmap::SetCamera(mat4 aCamera)
{
	SetDirty(true);
	mUniformCamera->Set(this, aCamera);
}
