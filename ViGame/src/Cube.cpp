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

void vigame::Cube::GetFace(CubeFace aFace, std::vector<ViVertex>& aVertices, std::vector<GLuint>& aIndices)
{
	/*if (mTransparent || GetMesh() == nullptr)
		return;

	const int vertSize = 4;
	const int indSize = 6;

	ViMesh* mesh = GetMesh();
	switch (aFace)
	{
	case CubeFace::cFACE_TOP:
	{
		int start = 4;
		int offVert = vertSize * start;
		int offInd = indSize * start;

		auto vertIterBegin = mesh->GetVertices().begin() + offVert;
		auto indIterBegin = mesh->GetVertices().begin() + offInd;

		aVertices.insert(aVertices.begin(), vertIterBegin, vertIterBegin + vertSize);
		aIndices.insert(aIndices.begin(), indIterBegin, indIterBegin + indSize);
		break;
	}
	case CubeFace::cFACE_BOTTOM:
	{
		int start = 5;
		int offVert = vertSize * start;
		int offInd = indSize * start;

		auto vertIterBegin = mesh->GetVertices().begin() + offVert;
		auto indIterBegin = mesh->GetVertices().begin() + offInd;

		aVertices.insert(aVertices.begin(), vertIterBegin, vertIterBegin + vertSize);
		aIndices.insert(aIndices.begin(), indIterBegin, indIterBegin + indSize);
		break;
	}
	case CubeFace::cFACE_LEFT:
	{
		int start = 3;
		int offVert = vertSize * start;
		int offInd = indSize * start;

		auto vertIterBegin = mesh->GetVertices().begin() + offVert;
		auto indIterBegin = mesh->GetVertices().begin() + offInd;

		aVertices.insert(aVertices.begin(), vertIterBegin, vertIterBegin + vertSize);
		aIndices.insert(aIndices.begin(), indIterBegin, indIterBegin + indSize);
		break;
	}
	case CubeFace::cFACE_RIGHT:
	{
		int start = 1;
		int offVert = vertSize * start;
		int offInd = indSize * start;

		auto vertIterBegin = mesh->GetVertices().begin() + offVert;
		auto indIterBegin = mesh->GetVertices().begin() + offInd;

		aVertices.insert(aVertices.begin(), vertIterBegin, vertIterBegin + vertSize);
		aIndices.insert(aIndices.begin(), indIterBegin, indIterBegin + indSize);
		break;
	}
	case CubeFace::cFACE_FRONT:
	{
		int start = 0;
		int offVert = vertSize * start;
		int offInd = indSize * start;

		auto vertIterBegin = mesh->GetVertices().begin() + offVert;
		auto indIterBegin = mesh->GetVertices().begin() + offInd;

		aVertices.insert(aVertices.begin(), vertIterBegin, vertIterBegin + vertSize);
		aIndices.insert(aIndices.begin(), indIterBegin, indIterBegin + indSize);
		break;
	}
	case CubeFace::cFACE_BACK:
	{
		int start = 2;
		int offVert = vertSize * start;
		int offInd = indSize * start;

		auto vertIterBegin = mesh->GetVertices().begin() + offVert;
		auto indIterBegin = mesh->GetVertices().begin() + offInd;

		aVertices.insert(aVertices.begin(), vertIterBegin, vertIterBegin + vertSize);
		aIndices.insert(aIndices.begin(), indIterBegin, indIterBegin + indSize);
		break;
	}
	}*/
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
	GetMesh()->UploadData();
}