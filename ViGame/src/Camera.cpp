#include "Camera.h"

#include "ViVertexBatch.h"
#include "ViAssetHandler.h"
#include "ViEnvironment.h"

vigame::Camera::Camera(ViTransform aStartTransform) :
	mTransform(aStartTransform)
{
	mProgramGeneric = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic"));
	mProgramLitGeneric = static_cast<ProgramLitGeneric*>(GET_ASSET_PROGRAM("lit_generic"));
	mProgramGenericFullBright = static_cast<ViProgramGeneric*>(GET_ASSET_PROGRAM("generic_fullbright"));

	mProgramGeneric->SetAmbientStrength(0.1f);
}

void vigame::Camera::Update(float deltaTime)
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

void vigame::Camera::LateUpdate(float aDeltaTime)
{
	ViTransform t = mTransform;
	t.SetPosition(-t.GetPosition());
	t.SetRotation(-t.GetRotation());

	mat4 mat = t.Matrix();
	mProgramGeneric->SetCamera(mat);
	mProgramLitGeneric->SetCamera(mat);
	mProgramLitGeneric->SetLightPosition(mTransform.GetPosition());
	//mProgramGeneric->SetDiffusePos(-mTransform.GetPosition());
	mProgramGenericFullBright->SetCamera(mat);
}

void vigame::Camera::Transform(vec3 aPosition, vec3 aRotation)
{
	mTransform.SetPosition(-aPosition);
	mTransform.SetRotation(-aRotation);
}
