#include "Player.h"

#include "VoxelWorld.h"
#include "Camera.h"
#include "ViVertexBatch.h"

vigame::Player::Player(ViTransform aStartTransform, VoxelWorld* aWorld, Camera* aCamera) :
	mTransform(aStartTransform),
	mWorld(aWorld),
	mCamera(aCamera),
	mNoclip(false)
{
	mCubeMesh = ViMesh::MakeUCube(ASSET_HANDLER->LoadMaterial("white_pixel_fullbright"), vec3(-0.5), vec3(0.5), ViMesh::cFACE_ALL, vicolors::WHITE);
	mCubeMesh->UploadData();

	mMaterialFont = new ViMaterialFont(GET_ASSET_FONT("debug"), GET_ASSET_MATERIAL("font_debug"));
}

void vigame::Player::Update(double aDeltaTime)
{
	float moveMult = -20 * aDeltaTime;
	float mouseSens = -100;

	if (INPUT_MANAGER->KeyDown(SDL_SCANCODE_N))
		mNoclip = !mNoclip;

	ViTransform trans = mTransform;
	if (mNoclip)
		trans = mCamera->GetTransform();

	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_W))
		trans.Translate(trans.Forward() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_A))
		trans.Translate(trans.Left() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_S))
		trans.Translate(trans.Backwards() * moveMult);
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_D))
		trans.Translate(trans.Right() * moveMult);

	mTransform = trans;

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
	
	rotation.x = 0;
	mTransform.SetRotation(rotation);

	mCamera->SetTransform(aCameraTransform);

	mVelocity *= drag * (float)aDeltaTime;

	//CollisionDetect();

	vec3i out;
	bool hit = !mWorld->VoxelRaycast(mTransform.GetPosition(), vec3(mTransform.GetPosition() - (mCamera->GetTransform().Forward() * 128.f)), out, [this](vec3i aPosition) {
		return mWorld->GetCubeInstance(aPosition).mId != 0;
	});

	if (hit)
	{
		mHighlighted = true;
		mHighlightedCube = out;

		if (INPUT_MANAGER->KeyDown(SDL_SCANCODE_G))
			mWorld->SetCubeInstance(out, 0);
	}
	else mHighlighted = false;
}

void vigame::Player::Draw(ViVertexBatch * aBatch)
{
	if (mHighlighted)
	{
		VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_LESS,
			ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_LINES));

		float scale = 1.f / mWorld->GetGridSize() + 0.001f;
		ViTransform trans = ViTransform::Positioned((vec3(mWorld->GetGridSize() / 2.0f) + vec3(mHighlightedCube)) / scale);
		trans.SetScale(vec3(scale));
		VERTEX_BATCH->Draw(trans, mCubeMesh, 6);

		VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_NONE,
			ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_ALPHABLEND, ViVertexBatchSettings::cDRAW_FILLED));
		aBatch->DrawString(ViTransform::Positioned({ 0, 128, 0 }), mMaterialFont, "Highlighted Cube Pos: x " + 
			std::to_string(mHighlightedCube.x) + ", y " + std::to_string(mHighlightedCube.y) + ", z " + std::to_string(mHighlightedCube.z) +
			", id: " + std::to_string(mWorld->GetCubeInstance(mHighlightedCube).mId));
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
