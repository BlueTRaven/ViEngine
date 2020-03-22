#include "VoxelWorld.h"

#include "ViEnvironment.h"

vigame::VoxelWorld::VoxelWorld(vec3i aSize, float aGridSize) :
	mSize(aSize),
	mGridSize(aGridSize),
	mCachedCube(nullptr),
	mCachedCubeid(-1),
	mCubes((CubeInstance*)malloc(aSize.x * aSize.y * aSize.z))
{
	//Cube with no mesh
	AddCube(new Cube(this, nullptr));
	AddCube(new Cube(this, GET_ASSET_MATERIAL("dirt_01")));

	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int z = 0; z < DEPTH; z++)
			{
				SetCubeInstance({ x, y, z }, GetCube(0));
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
	return CubeInstance(GetId(aCube));
}

void vigame::VoxelWorld::Draw(ViVertexBatch* batch)
{
	for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int z = 0; z < DEPTH; z++)
			{
				Cube* cube = GetCube(GetCubeInstance({ x, y, z }).mId);
				if (cube->GetMesh() != nullptr)
					batch->DrawMesh(cube->GetMesh(), ViTransform::Positioned(vec3( x, y, z) * GetGridSize()));
			}
		}
	}
}

void vigame::VoxelWorld::AddCube(Cube* aCube)
{
	cubeid id = (cubeid)mCubeTypes.size();
	aCube->SetId(id);
	mCubeTypes.push_back(aCube);
	mIdCubeMappings.emplace(aCube, id);
}

vigame::cubeid vigame::VoxelWorld::GetId(Cube* aCube)
{
	if (mCachedCubeid == -1 || mCachedCube == nullptr || aCube != mCachedCube)
	{
		mCachedCubeid = mIdCubeMappings[aCube];
		mCachedCube = aCube;
	}

	return mCachedCubeid;
}

vigame::Cube* vigame::VoxelWorld::GetCube(cubeid aId)
{
	if (mCachedCubeid == -1 || mCachedCube == nullptr || aId != mCachedCubeid)
	{
		mCachedCube = mCubeTypes[aId];
		mCachedCubeid = aId;
	}
	
	return mCachedCube;
}
