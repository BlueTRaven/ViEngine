#include "Player.h"

#include "VoxelWorld.h"
#include "Camera.h"
#include "ViVertexBatch.h"

vigame::Player::Player(ViTransform aStartTransform, VoxelWorld* aWorld, Camera* aCamera) :
	mTransform(aStartTransform),
	mWorld(aWorld),
	mCamera(aCamera),
	mNoclip(false),
	mVelocity(vec3(0)),
	mAcceleration(5)
{
	mCubeMesh = ViMesh::MakeUCube(ASSET_HANDLER->LoadMaterial("white_pixel_fullbright"), vec3(-0.5), vec3(0.5), ViMesh::cFACE_ALL, vicolors::WHITE);
	mCubeMesh->UploadData();

	mMaterialFont = new ViMaterialFont(GET_ASSET_FONT("debug"), GET_ASSET_MATERIAL("font_debug"));

	mMaxVelocity = vec3(20, 20, 20);

	mNoclip = true;
}

void vigame::Player::Update(double aDeltaTime)
{
	if (INPUT_MANAGER->KeyDown(SDL_SCANCODE_N))
		mNoclip = !mNoclip;

	mVelocity.x *= mDrag;
	mVelocity.z *= mDrag;
	//The only force exerted on y velocity is gravity, so don't apply drag
	//Unless we're in noclip - because otherwise we just wouldn't stop...
	if (mNoclip)
		mVelocity.y *= mDrag;

	if (!mNoclip)
		CollisionDetect();

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
	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_LESS,
		ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_LINES));
	
	vec3 forward = mTransform.Forward() * 1.f;
	ViTransform trans = ViTransform::Positioned(mTransform.GetPosition() + forward);
	VERTEX_BATCH->Draw(trans, mCubeMesh, 6);

	if (mHighlighted)
	{
		float scale = 1.f / mWorld->GetGridSize() + 0.001f;
		trans = ViTransform::Positioned((vec3(mWorld->GetGridSize() / 2.0f) + vec3(mHighlightedCube)) / scale);
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
				vec3i pos = (-mTransform.GetPosition() / mWorld->GetGridSize()) + vec3(x, y, z);
				CubeInstance instance = mWorld->GetCubeInstance(pos);

				if (instance.mId == 0)
					continue;

				vec3 posWorldSpace = vec3(pos) * mWorld->GetGridSize() - (mWorld->GetGridSize() / 2);

				float dist = -mTransform.GetPosition().y - posWorldSpace.y;

				//vec3 t = vec3(mTransform.GetPosition().x, -posWorldSpace.y, mTransform.GetPosition().z);
				if (dist < mWorld->GetGridSize())
					SetPosition(mTransform.GetPosition() - vec3(0, dist, 0));
			}
		}
	}
}

void vigame::Player::SetPosition(vec3 aPosition)
{
	mTransform.SetPosition(aPosition);
	ViTransform camTrans = mCamera->GetTransform();
	camTrans.SetPosition(aPosition);
	mCamera->SetTransform(camTrans);
}

void vigame::Player::Move(double aDeltaTime)
{
	float moveMult = -mAcceleration;
	float mouseSens = -100;

	ViTransform trans = mTransform;
	if (mNoclip)
		trans = mCamera->GetTransform();

	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_W))
		mVelocity += trans.Forward() * moveMult;
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_A))
		mVelocity += trans.Left() * moveMult;
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_S))
		mVelocity += trans.Backwards() * moveMult;
	if (INPUT_MANAGER->KeyHeld(SDL_SCANCODE_D))
		mVelocity += trans.Right() * moveMult;

	if (!mNoclip)
		mVelocity.y += 9.81f;

	vec3 velXZ = mVelocity;
	velXZ.y = 0;

	if (glm::length(velXZ) > glm::length(mMaxVelocity))
		velXZ = glm::normalize(velXZ) * glm::length(mMaxVelocity);

	mVelocity.x = velXZ.x;
	mVelocity.z = velXZ.z;

	if (mVelocity.y < -mMaxVelocity.y)
		mVelocity.y = -mMaxVelocity.y;
	else if (mVelocity.y > mMaxVelocity.y)
		mVelocity.y = mMaxVelocity.y;

	mTransform.Translate(mVelocity * (float)aDeltaTime);

	ViTransform aCameraTransform = mTransform;

	//TODO correct camera rotation
	//Right now it's going to go a little faster on the y axis since it's not properly normalized
	vec2 mDelta = INPUT_MANAGER->GetDistanceFromCenter(ENVIRONMENT->GetWindow());
	vec2 mNormalized = mDelta / (vec2(ENVIRONMENT->GetScreenWidth(), ENVIRONMENT->GetScreenHeight()) / vec2(2));
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

	mCamera->Transform(aCameraTransform.GetPosition(), aCameraTransform.GetRotation());
	//mCamera->SetTransform(aCameraTransform);
}
