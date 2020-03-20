#include "VoxelWorld.h"

vigame::VoxelWorldGenerator::VoxelWorldGenerator(vec3i aSize) :
	mSize(aSize),
	mCubes((CubeInstance*)malloc(aSize.x * aSize.y * aSize.z))
{
	//AddCube(new Cube(new ViMaterial()), 0);

	/*for (int x = 0; x < WIDTH; x++)
	{
		for (int y = 0; y < HEIGHT; y++)
		{
			for (int z = 0; z < DEPTH; z++)
			{
				SetCubeInstance({ x, y, z }, GetCube(0));
			}
		}
	}*/
}

void vigame::VoxelWorldGenerator::SetCubeInstance(vec3i aPosition, Cube* aCube)
{
	mCubes[aPosition.x + WIDTH * (aPosition.y + DEPTH * aPosition.z)] = MakeInstance(aCube);
}

vigame::CubeInstance& vigame::VoxelWorldGenerator::GetCubeInstance(vec3i aPosition)
{
	return mCubes[aPosition.x + WIDTH * (aPosition.y + DEPTH * aPosition.z)];
}

vigame::CubeInstance vigame::VoxelWorldGenerator::MakeInstance(Cube* aCube)
{
	return CubeInstance(GetId(aCube));
}

void vigame::VoxelWorldGenerator::Draw(ViVertexBatch* batch)
{
}

void vigame::VoxelWorldGenerator::AddCube(Cube* aCube, cubeid aId)
{
	mCubeIdMappings.emplace(0, aCube);
	mIdCubeMappings.emplace(aCube, 0);
}

vigame::cubeid vigame::VoxelWorldGenerator::GetId(Cube * aCube)
{
	return mIdCubeMappings[aCube];
}

vigame::Cube * vigame::VoxelWorldGenerator::GetCube(cubeid aId)
{
	return mCubeIdMappings[aId];
}
