#include "ProgramText.h"

#include "ViEnvironment.h"

//ViShader::Load(ViShaderType::VERTEX, "./Assets/Shaders/vertex_text.glsl"),
//ViShader::Load(ViShaderType::FRAGMENT, "./Assets/Shaders/fragment_text.glsl")
ViProgramText::ViProgramText() : 
	ViProgram(ENVIRONMENT->GetAssetHandler()->LoadShader("vertex_text"),
		ENVIRONMENT->GetAssetHandler()->LoadShader("fragment_text"),
		std::vector<ViVertexAttribute> {
			ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
			ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
			ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
		}),
	mUniformProjection(new ViUniformMat4("projection", glm::ortho(0.0, double(ENVIRONMENT->GetScreenWidth()), 0.0, double(ENVIRONMENT->GetScreenHeight()), -1.0, 1.0))),
	mUniformTintColor(new ViUniformVec4("tint_color", vicolors::WHITE.ToVec4())) 
{
}

void ViProgramText::SetUniforms(ViVertexBatchInstance& aInstance)
{
	if (!GetDirty())
		return;

	mUniformProjection->Upload(this);
	mUniformTintColor->Upload(this);

	ViProgram::SetUniforms(aInstance);
}

void ViProgramText::SetProjection(glm::mat4 aProjection)
{
	SetDirty(true);
	mUniformProjection->Set(this, aProjection);
}

void ViProgramText::SetColor(ViColorGL aColor)
{
	SetDirty(true);
	mUniformTintColor->Set(this, aColor.ToVec4());
}
