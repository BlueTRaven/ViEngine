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
	mCubeRegistry->AddCubeType(new Cube(this, nullptr));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("dirt_01")));

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int z = 0; z < DEPTH; z++)
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
	return mCubes[aPosition.x + WIDTH * (aPosition.y + DEPTH * aPosition.z)];
}

vigame::CubeInstance vigame::VoxelWorld::MakeInstance(Cube* aCube)
{
	return CubeInstance(mCubeRegistry->GetCubeId(aCube));
}

void vigame::VoxelWorld::Draw(ViVertexBatch* batch)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int z = 0; z < DEPTH; z++)
			{
				Cube* cube = mCubeRegistry->GetCubeType(GetCubeInstance({ x, y, z }).mId);
				if (cube->GetMesh() != nullptr)
					batch->Draw(ViTransform::Positioned(vec3(x, y, z) * GetGridSize()), cube->GetMesh());
			}
		}
	}
}
