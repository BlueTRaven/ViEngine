#include "Camera.h"

#include "ViVertexBatch.h"
#include "ViAssetHandler.h"
#include "ViEnvironment.h"

vigame::Camera::Camera(ViTransform aStartTransform) :
	mTransform(aStartTransform)
{
	mProgramGeneric = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic"));
	mProgramGenericFullBright = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic_fullbright"));

	mProgramGeneric->SetAmbientStrength(0.1f);
}

void vigame::Camera::Update(float deltaTime)
{
}

void vigame::Camera::LateUpdate(float aDeltaTime)
{
	mat4 mat = mTransform.Matrix();
	mProgramGeneric->SetCamera(mat);
	mProgramGeneric->SetDiffusePos(-mTransform.GetPosition());
	mProgramGenericFullBright->SetCamera(mat);
}
