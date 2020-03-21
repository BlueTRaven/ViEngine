#include "ProgramText.h"

#include "ViEnvironment.h"

//ViShader::Load(ViShaderType::VERTEX, "./Assets/Shaders/vertex_text.glsl"),
//ViShader::Load(ViShaderType::FRAGMENT, "./Assets/Shaders/fragment_text.glsl")
ViProgramText::ViProgramText() : 
	ViProgram(viEnv->GetAssetHandler()->LoadShader("vertex_text"),
		viEnv->GetAssetHandler()->LoadShader("fragment_text"),
		std::vector<ViVertexAttribute*> {
			new ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
			new ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
			new ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
		}),
	mProjection(glm::ortho(0.0, double(viEnv->GetScreenWidth()), 0.0, double(viEnv->GetScreenHeight()), -1.0, 1.0)),
	mColor(vicolors::WHITE) 
{
}

void ViProgramText::SetUniforms()
{
	UniformMat4(mProjection, false, "projection");
	UniformVec4(glm::vec4(mColor.r, mColor.g, mColor.b, mColor.a), "tint_color");
}

void ViProgramText::SetProjection(glm::mat4 aProjection)
{
	SetDirty(true);
	mProjection = aProjection;
}

void ViProgramText::SetColor(ViColorGL aColor)
{
	SetDirty(true);
	mColor = aColor;
}
