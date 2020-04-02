#include "Player.h"

#include "VoxelWorld.h"
#include "Camera.h"

vigame::Player::Player(ViTransform aStartTransform, VoxelWorld* aWorld, Camera* aCamera) :
	mTransform(aStartTransform),
	mWorld(aWorld),
	mCamera(aCamera)
{
}

void vigame::Player::Update(double aDeltaTime)
{
	float moveMult = -20 * aDeltaTime;
	float mouseSens = -100;

	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_W))
		mTransform.Translate(mTransform.Forward() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_A))
		mTransform.Translate(mTransform.Left() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_S))
		mTransform.Translate(mTransform.Backwards() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_D))
		mTransform.Translate(mTransform.Right() * moveMult);

	mTransform.Translate(mVelocity * (float)aDeltaTime);

	ViTransform aCameraTransform = mTransform;

	//TODO correct camera rotation
	//Right now it's going to go a little faster on the y axis since it's not properly normalized
	vec2 mDelta = INPUT_MANAGER->GetDistanceFromCenter(viEnv->GetWindow());
	vec2 mNormalized = mDelta / (vec2(viEnv->GetScreenWidth(), viEnv->GetScreenHeight()) / vec2(2));
	vec3 rotation = mCamera->GetTransform().GetRotation();
	rotation = rotation + vec3(mNormalized.y * mouseSens, mNormalized.x * mouseSens, 0.0);

	if (rotation.x > 85)
		rotation.x = 85;
	if (rotation.x < -85)
		rotation.x = -85;

	rotation.y = std::fmod(rotation.y, 360.0f);

	aCameraTransform.SetRotation(rotation);

	mCamera->SetTransform(aCameraTransform);

	mVelocity *= drag * (float)aDeltaTime;

	//CollisionDetect();

	vec3i out;
	bool hit = !mWorld->VoxelRaycast(mTransform.GetPosition(), vec3(mTransform.GetPosition() - (mTransform.Forward() * 128.f)), out, [this](vec3i aPosition) {
		return mWorld->GetCubeInstance(aPosition).mId != 0;
	});

	if (hit)
	{
		mHighlightedCube = out;

		if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_G))
		{
			if (!mAttackKey)
			{
				mAttackKey = true;
				mWorld->SetCubeInstance(out, 0);
				//highlightedChunk->SetDirty(true);
			}
		}
		else mAttackKey = false;
	}
}

void vigame::Player::CollisionDetect()
{
	for (int z = -1; z <= 1; z++)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				vec3i pos = (vec3i(mTransform.GetPosition()) / mWorld->GetSize()) + vec3i(x, y, z);
				CubeInstance instance = mWorld->GetCubeInstance(pos);

				if (instance.mId == 0)
					continue;

				vec3 posWorldSpace = vec3(pos * mWorld->GetSize());

				if (!mWorld->GetCubeRegistry()->GetCubeType(instance.mId)->GetTransparent())
				{
					vec3 dist = mTransform.GetPosition() - posWorldSpace + mWorld->GetGridSize();

					if (dist.y < 0)
						mTransform.Translate({ 0, dist.y, 0 });
				}
			}
		}
	}
}
