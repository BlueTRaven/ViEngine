#include "Cube.h"

#include "ViVertexBatch.h"
#include "VoxelWorld.h"
#include "CubeInstance.h"

vigame::Cube::Cube(VoxelWorld* world, ViMaterial* aMaterial, bool aTransparent) :
	mWorld(world),
	idSet(false),
	mTransparent(aTransparent)
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

uint8_t vigame::Cube::GetAdjacents(const CubeInstance& aCubeInstance, vec3i aPosition)
{
	VoxelWorld* world = GetWorld();

	uint8_t notFaces = 0;

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

void vigame::Cube::Draw(const CubeInstance& aCubeInstance, vec3i aPosition, ViVertexBatch* aBatch)
{
	uint8_t adjacents = GetAdjacents(aCubeInstance, aPosition);

	if (adjacents & ViMesh::cFACE_FRONT)
		aBatch->Draw(ViTransform::Positioned(glm::vec3(aPosition) * mWorld->GetGridSize()), mMesh, 0);
	if (adjacents & ViMesh::cFACE_RIGHT)
		aBatch->Draw(ViTransform::Positioned(glm::vec3(aPosition) * mWorld->GetGridSize()), mMesh, 1);
	if (adjacents & ViMesh::cFACE_BACK)
		aBatch->Draw(ViTransform::Positioned(glm::vec3(aPosition) * mWorld->GetGridSize()), mMesh, 2);
	if (adjacents & ViMesh::cFACE_LEFT)
		aBatch->Draw(ViTransform::Positioned(glm::vec3(aPosition) * mWorld->GetGridSize()), mMesh, 3);
	if (adjacents & ViMesh::cFACE_TOP)
		aBatch->Draw(ViTransform::Positioned(glm::vec3(aPosition) * mWorld->GetGridSize()), mMesh, 4);
	if (adjacents & ViMesh::cFACE_BOTTOM)
		aBatch->Draw(ViTransform::Positioned(glm::vec3(aPosition) * mWorld->GetGridSize()), mMesh, 5);
}

vigame::VoxelWorld* vigame::Cube::GetWorld()
{
	return mWorld;
}

void vigame::Cube::CreateMesh(ViMaterial* aMaterial)
{
	if (aMaterial == nullptr)
		return;

	float scale = GetWorld()->GetGridSize() / 2;	//since we start in the middle...
	vec3 min = { -scale, scale, scale };
	vec3 max = { scale, -scale, -scale };

	mMesh = ViMesh::MakeUCube(aMaterial, min, max, ViMesh::cFACE_ALL);
	if (mMesh != ViMesh::GetEmpty())
		mMesh->UploadData();
	/*for (uint8_t i = 0; i <= ViMesh::cFACE_ALL; i++)
	{
		ViMesh* mesh = ViMesh::MakeUCube(aMaterial, min, max, i);
		if (mesh != ViMesh::GetEmpty())
			mesh->UploadData();
		mFaceMeshes.push_back(mesh);
	}*/
}

bool vigame::Cube::GetAdjacentCubeShouldHideFace(vec3i aPosition)
{
	CubeInstance instance = GetWorld()->GetCubeInstance(aPosition);
	if (instance.mId == 0)
		return false;

	if (GetWorld()->GetCubeRegistry()->GetCubeType(instance.mId)->GetTransparent() && !mTransparent)
		return false;

	return true;

}
