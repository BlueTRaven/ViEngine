#include "Cube.h"

#include "ViVertexBatch.h"
#include "VoxelWorld.h"
#include "CubeInstance.h"

vigame::Cube::Cube(VoxelWorld* world, ViColorGL aColor, bool aTransparent) :
	mWorld(world),
	idSet(false),
	mTransparent(aTransparent),
	mColor(aColor)
{
	//CreateMesh(aMaterial);
}

void vigame::Cube::SetId(cubeid aId)
{
	vi_assert(!idSet, "Error: Cannot re-set id.");

	idSet = true;
	mId = aId;
}

int vigame::Cube::GetAdjacents(const CubeInstance& aCubeInstance, vec3i aPosition)
{
	VoxelWorld* world = GetWorld();

	int notFaces = 0;

	//TODO check transparent cubes
	if (GetAdjacentCubeShouldHideFace(aPosition + vec3i(1, 0, 0)))
		notFaces = notFaces | ViMesh::cFACE_RIGHT;
	if (GetAdjacentCubeShouldHideFace(aPosition + vec3i(-1, 0, 0)))
		notFaces = notFaces | ViMesh::cFACE_LEFT;
	if (GetAdjacentCubeShouldHideFace(aPosition + vec3i(0, 1, 0)))
		notFaces = notFaces | ViMesh::cFACE_TOP;
	if (GetAdjacentCubeShouldHideFace(aPosition + vec3i(0, -1, 0)))
		notFaces = notFaces | ViMesh::cFACE_BOTTOM;
	if (GetAdjacentCubeShouldHideFace(aPosition + vec3i(0, 0, 1)))
		notFaces = notFaces | ViMesh::cFACE_FRONT;
	if (GetAdjacentCubeShouldHideFace(aPosition + vec3i(0, 0, -1)))
		notFaces = notFaces | ViMesh::cFACE_BACK;

	return ~notFaces & ViMesh::cFACE_ALL;
}

std::vector<int> vigame::Cube::GetSubsectionsToDraw(const CubeInstance& aCubeInstance, vec3i aPosition)
{
	uint8_t adjacents = GetAdjacents(aCubeInstance, aPosition);
	if (adjacents == 0)
		return std::vector<int>();

	std::vector<int> subsections;
	if (adjacents & ViMesh::cFACE_FRONT)
		subsections.push_back(0);
	if (adjacents & ViMesh::cFACE_RIGHT)
		subsections.push_back(1);
	if (adjacents & ViMesh::cFACE_BACK)
		subsections.push_back(2);
	if (adjacents & ViMesh::cFACE_LEFT)
		subsections.push_back(3);
	if (adjacents & ViMesh::cFACE_TOP)
		subsections.push_back(4);
	if (adjacents & ViMesh::cFACE_BOTTOM)
		subsections.push_back(5);

	return subsections;
}

ViTransform vigame::Cube::GetWorldSpaceTransform(const CubeInstance& aCubeInstance, vec3i aPosition)
{
	return ViTransform::Positioned(glm::vec3(aPosition) * mWorld->GetGridSize());
}

vigame::VoxelWorld* vigame::Cube::GetWorld()
{
	return mWorld;
}

bool vigame::Cube::GetAdjacentCubeShouldHideFace(vec3i aPosition)
{
	CubeInstance instance = GetWorld()->GetCube(aPosition);
	if (instance.mId == 0)
		return false;

	if (GetWorld()->GetCubeRegistry()->GetCubeType(instance.mId)->GetTransparent() && !mTransparent)
		return false;

	return true;

}
