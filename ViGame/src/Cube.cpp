#include "Cube.h"

#include "VoxelWorld.h"

vigame::Cube::Cube(VoxelWorld* world, ViMaterial* aMaterial) :
	mWorld(world),
	idSet(false)
{
	CreateMesh(aMaterial);
}

void vigame::Cube::SetId(cubeid aId)
{
	vi_assert(!idSet, "Error: Cannot re-set id.");

	idSet = true;
	mId = aId;
}

inline vigame::cubeid vigame::Cube::GetId()
{
	return mId;
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