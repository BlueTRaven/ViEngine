#include "VoxelWorld.h"

#include "ViEnvironment.h"
#include "ViVertexBatch.h"

vigame::VoxelWorld::VoxelWorld(vec3i aSize, float aGridSize, WorldGenerator* aWorldGenerator) :
	mSize(aSize),
	mCubeRegistry(new CubeRegistry()),
	mGridSize(aGridSize),
	mCubes((CubeInstance*)malloc((aSize.x * aSize.y * aSize.z) * sizeof(CubeInstance))),
	mGenerator(aWorldGenerator),
	mDrawDebug(false)
{
	mProgramCubesInstanced = static_cast<ProgramCubesInstanced*>(GET_ASSET_PROGRAM("cube_instanced"));

	vi_assert((aSize.x % Chunk::cWIDTH) == 0, "Size of world must be divisible by Chunk::cWIDTH.");
	vi_assert((aSize.y % Chunk::cHEIGHT) == 0, "Size of world must be divisible by Chunk::cHEIGHT.");
	vi_assert((aSize.z % Chunk::cDEPTH) == 0, "Size of world must be divisible by Chunk::cDEPTH.");

	mChunkSize = vec3i(aSize.x / Chunk::cWIDTH, aSize.y / Chunk::cHEIGHT, aSize.z / Chunk::cDEPTH);

	for (int i = 0; i < (mChunkSize.x * mChunkSize.y * mChunkSize.z); i++)
	{
		vec3i pos = IndexTo3DIndex(i, mChunkSize);
		Chunk* chunk = new Chunk(pos, this);

		mChunksA.push_back(chunk);
	}

	//Cube with no mesh
	mCubeRegistry->AddCubeType(new Cube(this, nullptr, false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("white_pixel"), false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("glass_01"), true));

	//set all ids to 0
	memset(mCubes, 0, (aSize.x * aSize.y * aSize.z) * sizeof(CubeInstance));

	mGenerator->Init(this);
	for (int i = 0; i < (mChunkSize.x * mChunkSize.y * mChunkSize.z); i++)
	{
		vec3i pos = IndexTo3DIndex(i, mChunkSize);
		mGenerator->GenerateChunk(pos);
	}
}

void vigame::VoxelWorld::SetCubeInstance(vec3i aPosition, Cube* aCube)
{
	//mProgramCubesInstanced->SetInstance(aPosition, aCube->GetId(), mCubes[Vec3IndexToIndex(aPosition, mSize)].mId);
	mCubes[Vec3IndexToIndex(aPosition, mSize)] = MakeInstance(aCube);

	Chunk* chunk = GetChunkResponsibleForCube(aPosition);
	if (chunk == nullptr)
		chunk = MakeChunk(CubeSpaceToChunkSpace(aPosition));
	chunk->SetDirty(true);
}

vigame::CubeInstance& vigame::VoxelWorld::GetCubeInstance(vec3i aPosition)
{
	if (aPosition.x < 0 || aPosition.y < 0 || aPosition.z < 0 || aPosition.x >= mSize.x || aPosition.y >= mSize.y || aPosition.z >= mSize.z)
		return voidCube;

	return mCubes[Vec3IndexToIndex(aPosition, mSize)];
}

vigame::CubeInstance vigame::VoxelWorld::MakeInstance(Cube* aCube)
{
	return CubeInstance(mCubeRegistry->GetCubeId(aCube));
}

vigame::Chunk* vigame::VoxelWorld::GetChunk(vec3i aChunkPosition)
{
	if (aChunkPosition.x < 0 || aChunkPosition.y < 0 || aChunkPosition.z < 0 || aChunkPosition.x > mSize.x || aChunkPosition.y > mSize.y || aChunkPosition.z > mSize.z)
		return nullptr;

	return mChunksA[Vec3IndexToIndex(aChunkPosition, mChunkSize)];
}

vigame::Chunk* vigame::VoxelWorld::MakeChunk(vec3i aChunkPosition)
{
	Chunk* chunk = new Chunk(aChunkPosition, this);
	mChunksA[Vec3IndexToIndex(aChunkPosition, mChunkSize)] = chunk;
	return chunk;
}

vigame::Chunk* vigame::VoxelWorld::GetChunkResponsibleForCube(vec3i aPosition)
{
	vec3i pos = CubeSpaceToChunkSpace(aPosition);

	return GetChunk(pos);
}

vec3i vigame::VoxelWorld::CubeSpaceToChunkSpace(vec3i aPosCubeSpace)
{
	return vec3i(aPosCubeSpace.x / Chunk::cWIDTH, aPosCubeSpace.y / Chunk::cHEIGHT, aPosCubeSpace.z / Chunk::cDEPTH);
}

vec3i vigame::VoxelWorld::ChunkSpaceToCubeSpace(vec3i aPosChunkSpace)
{
	return vec3i(aPosChunkSpace.x * Chunk::cWIDTH, aPosChunkSpace.y * Chunk::cHEIGHT, aPosChunkSpace.z * Chunk::cDEPTH);
}

vec3 vigame::VoxelWorld::CubeSpaceToWorldSpace(vec3i aPosCubeSpace)
{
	return vec3(aPosCubeSpace) * vec3(mGridSize);
}

void vigame::VoxelWorld::Update(float aDeltaTime)
{
	
}

void vigame::VoxelWorld::Draw(ViVertexBatch* aBatch)
{
	//mChunksA[0]->Draw(aBatch);
	for (int i = 0; i < mChunksA.size(); i++)
	{
		mChunksA[i]->Draw(aBatch);
	}

	//Instanced test
	//aBatch->Draw(ViTransform::Positioned({ 0, 0, 0 }), GetCubeRegistry()->GetCubeType(1)->GetMesh(), 6, 1);

	if (mDrawDebug)
	{
		VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_LESS,
			ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_LINES));
		for (int i = 0; i < mChunksA.size(); i++)
		{
			aBatch->Draw(ViTransform::Positioned({ 0, 0, 0 }), mChunksA[i]->GetWireframeMesh());
		}
	}
}
