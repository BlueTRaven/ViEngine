#include "ProgramLitGeneric.h"

#include "ViEnvironment.h"
#include "ViAssetHandler.h"

vigame::ProgramLitGeneric::ProgramLitGeneric() : ViProgram(ASSET_HANDLER->LoadShader("vertex_lit_generic"), ASSET_HANDLER->LoadShader("fragment_lit_generic"), std::vector<ViVertexAttribute> {
	ViVertexAttribute("normal", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, nrm)),
		ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
		ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
		ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
}),
mUniformProjection(new ViUniformMat4("projection", glm::perspective(glm::radians(80.0), double(ENVIRONMENT->GetScreenWidth()) / double(ENVIRONMENT->GetScreenHeight()), 0.01, 1000.0))),
mUniformCamera(new ViUniformMat4("camera", glm::identity<glm::mat4>())),
mUniformLightPos(new ViUniformVec3("diffuse_light.position", vec3(0))),
mUniformLightColor(new ViUniformVec3("diffuse_light.color", vec3(1, 1, 1))),
mUniformLightStrength(new ViUniformFloat("diffuse_light.strength", 1)),
mUniformRadialFogColor(new ViUniformVec3("radial_fog.color", vec3(1))),
mUniformRadialFogStart(new ViUniformFloat("radial_fog.start", 64)),
mUniformRadialFogEnd(new ViUniformFloat("radial_fog.end", 128))
{
}

void vigame::ProgramLitGeneric::SetUniforms(ViVertexBatchInstance & aInstance)
{
	mUniformProjection->Upload(this);
	mUniformCamera->Upload(this);

	mUniformLightPos->Upload(this);
	mUniformLightColor->Upload(this);
	mUniformLightStrength->Upload(this);

	mUniformRadialFogColor->Upload(this);
	mUniformRadialFogStart->Upload(this);
	mUniformRadialFogEnd->Upload(this);

	ViProgram::SetUniforms(aInstance);
}

void vigame::ProgramLitGeneric::SetDiffuseLight(DiffuseLight aLight)
{
	SetDirty(true);
	mUniformLightPos->Set(this, aLight.position);
	mUniformLightColor->Set(this, aLight.color);
	mUniformLightStrength->Set(this, aLight.strength);
}

void vigame::ProgramLitGeneric::SetRadialFog(RadialFog aFog)
{
	SetDirty(true);
	mUniformRadialFogColor->Set(this, aFog.color);
	mUniformRadialFogStart->Set(this, aFog.start);
	mUniformRadialFogEnd->Set(this, aFog.end);
}

void vigame::ProgramLitGeneric::SetCamera(mat4 aCamera)
{
	SetDirty(true);
	mUniformCamera->Set(this, aCamera);
}
