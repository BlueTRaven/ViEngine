#include "Camera.h"

#include "ViVertexBatch.h"
#include "ViAssetHandler.h"
#include "ViEnvironment.h"

vigame::Camera::Camera(ViTransform aStartTransform) :
	mTransform(aStartTransform),
	mControlMode(cCAMERA_CONTROLLED),
	mTargetPosition(vec3(0)),
	mTargetRotation(vec3(0)),
	mTargetEase(0.1f)
{
	mProgramGeneric = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic"));
	mProgramUnlitGeneric = static_cast<ProgramUnlitGeneric*>(GET_ASSET_PROGRAM("unlit_generic"));
	mProgramLitGeneric = static_cast<ProgramLitGeneric*>(GET_ASSET_PROGRAM("lit_generic"));
	mProgramShadowmap = static_cast<ProgramShadowmap*>(GET_ASSET_PROGRAM("shadowmap"));

	mProgramGeneric->SetAmbientStrength(0.1f);

	mDiffuseLight.position = mTransform.GetPosition();
	mDiffuseLight.color = vicolors::WHITE.ToVec4();
	mDiffuseLight.strength = 1;

	mRadialFog.color = vicolors::BLUE.ToVec4();
	mRadialFog.start = 64;
	mRadialFog.end = 128;
}

void vigame::Camera::Update(float deltaTime)
{
	if (mControlMode == cCAMERA_CONTROLLED)
	{
		float mouseSens = 100;
		float acceleration = 0.5f;

		if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_W))
			mTransform.Translate(mTransform.Forward() * acceleration);
		if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_A))
			mTransform.Translate(mTransform.Left() * acceleration);
		if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_S))
			mTransform.Translate(mTransform.Backwards() * acceleration);
		if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_D))
			mTransform.Translate(mTransform.Right() * acceleration);

		vec2 mDelta = INPUT_MANAGER->GetDistanceFromCenter(ENVIRONMENT->GetWindow());
		vec2 mNormalized = mDelta / (vec2(ENVIRONMENT->GetScreenWidth(), ENVIRONMENT->GetScreenHeight()) / vec2(2));
		vec3 rotation = GetTransform().GetRotation();
		rotation = rotation + vec3(mNormalized.y * mouseSens, mNormalized.x * mouseSens, 0.0);

		if (rotation.x > 85)
			rotation.x = 85;
		if (rotation.x < -85)
			rotation.x = -85;

		rotation.y = std::fmod(rotation.y, 360.0f);

		mTransform.SetRotation(rotation);
	}
	else if (mControlMode == cTARGET_CONTROLLED)
	{
		vec3 position = mTransform.GetPosition();
		vec3 rotation = mTransform.GetRotation();

		vec3 outPos = glm::mix(position, mTargetPosition, mTargetEase);
		vec3 outRot = glm::mix(rotation, mTargetRotation, mTargetEase);

		mTransform.SetPosition(outPos);
		mTransform.SetRotation(outRot);
	}
}

void vigame::Camera::LateUpdate(float aDeltaTime)
{
	ViTransform t = mTransform;
	t.SetPosition(-t.GetPosition());
	t.SetRotation(-t.GetRotation());

	mat4 mat = t.Matrix();
	mProgramGeneric->SetCamera(mat);
	mProgramLitGeneric->SetCamera(mat);

	mProgramLitGeneric->SetDiffuseLight(mDiffuseLight);
	mProgramLitGeneric->SetRadialFog(mRadialFog);
	mProgramUnlitGeneric->SetCamera(mat);

	t.Translate(t.Backwards() * 8.0f);
	mat = t.Matrix();
	mProgramShadowmap->SetCamera(mat);
}
