#include "Player.h"

#include "VoxelWorld.h"

vigame::Player::Player(VoxelWorld* aWorld) :
	mWorld(aWorld)
{
}

void vigame::Player::Update(double aDeltaTime)
{
	mTransform.Translate(mVelocity * (float)aDeltaTime);

	mVelocity *= drag * (float)aDeltaTime;

	CollisionDetect();
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
