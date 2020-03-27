#include "ProgramGeneric.h"

#include "ViEnvironment.h"

ViProgramGeneric::ViProgramGeneric() :
	ViProgram(viEnv->GetAssetHandler()->LoadShader("vertex_generic"),
		viEnv->GetAssetHandler()->LoadShader("fragment_generic"),
		std::vector<ViVertexAttribute*> {
	new ViVertexAttribute("normal", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, nrm)),
		new ViVertexAttribute("position", 3, sizeof(ViVertex), (void*)offsetof(ViVertex, pos)),
		new ViVertexAttribute("color", 4, sizeof(ViVertex), (void*)offsetof(ViVertex, color)),
		new ViVertexAttribute("tex_coord", 2, sizeof(ViVertex), (void*)offsetof(ViVertex, texCoord))
}),
mProjection(glm::perspectiveFov(45.0, double(viEnv->GetScreenWidth()), double(viEnv->GetScreenHeight()), 0.01, 1000.0)),
mCamera(glm::identity<glm::mat4>()),
mTintColor(vicolors::WHITE),
mAmbientColor(vicolors::WHITE),
mAmbientStrength(1.f),
mDiffusePos({ 0.0, 1.0, -1.0 })
{
}

void ViProgramGeneric::SetUniforms()
{
	if (!GetDirty())
		return;
	
	UniformMat4(mProjection, false, "projection");
	UniformMat4(mCamera, false, "camera");
	UniformVec4(glm::vec4(mTintColor.r, mTintColor.g, mTintColor.b, mTintColor.a), "tint_color");

	//Frag shader
	UniformVec3({ 1.0, 1.0, 1.0 }, "diffuse_color");
	UniformVec3(mDiffusePos, "diffuse_pos");

	UniformVec3(vec3(mAmbientColor.r, mAmbientColor.g, mAmbientColor.b), "ambient_color");
	UniformFloat(mAmbientStrength, "ambient_strength");
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

void ViProgramGeneric::SetTintColor(ViColorGL aColor)
{
	SetDirty(true);
	mTintColor = aColor;
}

void ViProgramGeneric::SetAmbientColor(ViColorGL aColor)
{
	SetDirty(true);
	mAmbientColor = aColor;
}

void ViProgramGeneric::SetAmbientStrength(float aStrength)
{
	SetDirty(true);
	mAmbientStrength = aStrength;
}

void ViProgramGeneric::SetDiffusePos(vec3 aPos)
{
	SetDirty(true);
	mDiffusePos = aPos;
}

void ViProgramGeneric::SetObjectMat(mat4 aObjMat)
{
	UniformMat4(glm::transpose(glm::inverse(mCamera * aObjMat)), false, "normal_object");

	ViProgram::SetObjectMat(aObjMat);
}
