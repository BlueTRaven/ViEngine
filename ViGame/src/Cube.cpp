#include "Cube.h"

#include "VoxelWorld.h"

vigame::Cube::Cube(VoxelWorld* world, ViMaterial* aMaterial) :
	mWorld(world)
{
	CreateMesh(aMaterial);
}

vigame::VoxelWorld* vigame::Cube::GetWorld()
{
	return mWorld;
}

void vigame::Cube::CreateMesh(ViMaterial* aMaterial)
{	
	if (aMaterial == nullptr)
	{
		SetMesh(nullptr);
		return;
	}

	float scale = GetWorld()->GetGridSize() / 2;	//since we start in the middle...
	vec3 min = { -scale, scale, scale };
	vec3 max = { scale, -scale, -scale };
	SetMesh(ViMesh::MakeUCube(aMaterial, min, max));
}