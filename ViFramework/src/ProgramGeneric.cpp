#include "ProgramGeneric.h"

#include "ViEnvironment.h"

// ViShader::Load(ViShaderType::VERTEX, "./Assets/Shaders/vertex_generic.glsl")
// ViShader::Load(ViShaderType::FRAGMENT, "./Assets/Shaders/fragment_generic.glsl")
ViProgramGeneric::ViProgramGeneric() : 
	ViProgram(viEnv->GetAssetHandler()->LoadShader("vertex_generic"), 
		viEnv->GetAssetHandler()->LoadShader("fragment_generic"),
		std::vector<ViVertexAttribute*> {
			new ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
			new ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
			new ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
		}),
	mProjection(glm::perspectiveFov(45.0, double(viEnv->GetScreenWidth()), double(viEnv->GetScreenHeight()), 0.01, 1000.0)),
	mCamera(glm::identity<glm::mat4>()),
	mColor(vicolors::WHITE)
{
}

void ViProgramGeneric::SetUniforms()
{
	UniformMat4(mProjection, false, "projection");
	UniformMat4(mCamera, false, "camera");
	UniformVec4(glm::vec4(mColor.r, mColor.g, mColor.b, mColor.a), "tint_color");
}

void ViProgramGeneric::SetProjection(glm::mat4 aProjection)
{
	SetDirty(true);
	this->mProjection = aProjection;
}

void ViProgramGeneric::SetCamera(glm::mat4 aCamera)
{
	SetDirty(true);
	mCamera = aCamera;
}

void ViProgramGeneric::SetColor(ViColorGL aColor)
{
	SetDirty(true);
	mColor = aColor;
}
