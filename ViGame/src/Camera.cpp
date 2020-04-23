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
	mProgramLitGeneric = static_cast<ProgramLitGeneric*>(GET_ASSET_PROGRAM("lit_generic"));
	mProgramGenericFullBright = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic_fullbright"));

	mProgramGeneric->SetAmbientStrength(0.1f);
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
	
	auto diffuseLight = ProgramLitGeneric::DiffuseLight();
	diffuseLight.position = mTransform.GetPosition();
	diffuseLight.color = vicolors::WHITE.ToVec4();
	diffuseLight.strength = 1;

	auto radialFog = ProgramLitGeneric::RadialFog();
	radialFog.color = vicolors::BLACK.ToVec4();
	radialFog.start = 64;
	radialFog.end = 128;

	mProgramLitGeneric->SetDiffuseLight(diffuseLight);
	mProgramLitGeneric->SetRadialFog(radialFog);
	mProgramGenericFullBright->SetCamera(mat);
}
