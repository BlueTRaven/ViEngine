#include "VoxelWorld.h"

#include "ViEnvironment.h"
#include "ViVertexBatch.h"

vigame::VoxelWorld::VoxelWorld(vec3i aSize, float aGridSize) :
	mSize(aSize),
	mCubeRegistry(new CubeRegistry()),
	mGridSize(aGridSize),
	mCubes((CubeInstance*)malloc(aSize.x * aSize.y * aSize.z))
{
	//Cube with no mesh
	mCubeRegistry->AddCubeType(new Cube(this, nullptr, false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("dirt_01"), false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("glass_01"), true));

	for (int z = 0; z < DEPTH; ++z)
	{
		for (int y = 0; y < HEIGHT; ++y)
		{
			for (int x = 0; x < WIDTH; ++x)
			{
				SetCubeInstance({ x, y, z }, mCubeRegistry->GetCubeType(0));
			}
		}
	}
}

void vigame::VoxelWorld::SetCubeInstance(vec3i aPosition, Cube* aCube)
{
	mCubes[aPosition.x + WIDTH * (aPosition.y + DEPTH * aPosition.z)] = MakeInstance(aCube);
}

vigame::CubeInstance& vigame::VoxelWorld::GetCubeInstance(vec3i aPosition)
{
	if (aPosition.x < 0 || aPosition.y < 0 || aPosition.z < 0 || aPosition.x > WIDTH || aPosition.y > HEIGHT || aPosition.z > DEPTH)
		return mCubes[0];

	return mCubes[aPosition.x + WIDTH * (aPosition.y + DEPTH * aPosition.z)];
}

vigame::CubeInstance vigame::VoxelWorld::MakeInstance(Cube* aCube)
{
	return CubeInstance(mCubeRegistry->GetCubeId(aCube));
}

void vigame::VoxelWorld::Draw(ViVertexBatch* aBatch)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int z = 0; z < DEPTH; z++)
			{
				CubeInstance cubeInstance = GetCubeInstance({ x, y, z });
				if (cubeInstance.mId == 0)
					continue;

				Cube* cube = mCubeRegistry->GetCubeType(cubeInstance.mId);
				cube->Draw(cubeInstance, { x, y, z }, aBatch);
				/*uint8_t adjacents = cube->GetAdjacents(cubeInstance, { x, y, z });
				if (adjacents == 0)
					continue;

				ViMesh* mesh = cube->GetMeshWithFace(adjacents);

				if (mesh != nullptr)
					batch->Draw(ViTransform::Positioned(vec3(x, y, z) * GetGridSize()), mesh);*/
			}
		}
	}
}
