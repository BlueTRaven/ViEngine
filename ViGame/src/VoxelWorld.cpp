#include "VoxelWorld.h"

#include "ViEnvironment.h"
#include "ViVertexBatch.h"

vigame::VoxelWorld::VoxelWorld(vec3i aSize, float aGridSize, WorldGenerator* aWorldGenerator) :
	mSize(aSize),
	mCubeRegistry(new CubeRegistry()),
	mGridSize(aGridSize),
	mGenerator(aWorldGenerator),
	mDrawDebug(false),
	mLoadPosition(vec3(0)),
	mOldLoadPosition(vec3(0)),
	mTimer(0)
{
	mProgramCubesInstanced = static_cast<ProgramCubesInstanced*>(GET_ASSET_PROGRAM("cube_instanced"));

	vec3i chunkSize = Chunk::GetSize();

	vi_assert((aSize.x % chunkSize.x) == 0, "Size of world must be divisible by Chunk Width.");
	vi_assert((aSize.y % chunkSize.y) == 0, "Size of world must be divisible by Chunk Height.");
	vi_assert((aSize.z % chunkSize.z) == 0, "Size of world must be divisible by Chunk Depth.");

	mChunkSize = aSize / chunkSize;

	//Cube with no mesh
	mCubeRegistry->AddCubeType(new Cube(this, nullptr, vicolors::WHITE, false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("white_pixel"), vicolors::WHITE, false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("white_pixel"), vicolors::GREEN, true));

	mGenerator->Init(this);
	
	for (int z = 0; z < mChunkSize.z; z++)
	{
		for (int y = 0; y < mChunkSize.y; y++)
		{
			for (int x = 0; x < mChunkSize.x; x++)
			{
				MakeChunk(vec3i(x, y, z));
			}
		}
	}

	mCubeMesh = ViMesh::MakeUCube(ASSET_HANDLER->LoadMaterial("white_pixel_fullbright"), vec3(-0.5), vec3(0.5), ViMesh::cFACE_ALL, vicolors::WHITE);
	mCubeMesh->UploadData();
}

void vigame::VoxelWorld::SetCube(vec3i aPosition, Cube* aCube)
{
	Chunk* chunk = GetChunkResponsibleForCube(aPosition);

	if (chunk == nullptr)
		return;

	chunk->SetCubeRelative(MakeInstance(aCube->GetId()), aPosition - chunk->GetWorldPosition() * Chunk::GetSize());
}

vigame::CubeInstance& vigame::VoxelWorld::GetCube(vec3i aPosition)
{	
	Chunk* chunk = GetChunkResponsibleForCube(aPosition);

	if (chunk == nullptr)
		return voidCube;

	return chunk->GetCubeRelative(aPosition - chunk->GetWorldPosition() * Chunk::GetSize());
}

vigame::CubeInstance vigame::VoxelWorld::MakeInstance(Cube* aCube)
{
	return CubeInstance(mCubeRegistry->GetCubeId(aCube));
}

vigame::CubeInstance vigame::VoxelWorld::MakeInstance(cubeid aId)
{
	return CubeInstance(aId);
}

vigame::Chunk* vigame::VoxelWorld::GetChunk(vec3i aChunkPosition)
{
	if (aChunkPosition.x < 0 || aChunkPosition.y < 0 || aChunkPosition.z < 0 || aChunkPosition.x >= mChunkSize.x || aChunkPosition.y >= mChunkSize.y || aChunkPosition.z >= mChunkSize.z)
		return nullptr;

	if (mChunks.find(aChunkPosition) == mChunks.end())
		return nullptr;

	return mChunks[aChunkPosition];
}

vigame::Chunk* vigame::VoxelWorld::MakeChunk(vec3i aChunkWorldPosition)
{
	Chunk* chunk = new Chunk(aChunkWorldPosition, this);

	if (mChunks.find(aChunkWorldPosition) != mChunks.end())
		mChunks[aChunkWorldPosition] = chunk;
	else mChunks.emplace(aChunkWorldPosition, chunk);

	mGenerator->GenerateChunk(chunk);
	return chunk;
}

void vigame::VoxelWorld::RemoveChunk(vec3i aChunkPosition)
{
	delete GetChunk(aChunkPosition);
}

vigame::Chunk* vigame::VoxelWorld::GetChunkResponsibleForCube(vec3i aPosition)
{
	vec3i pos = CubeSpaceToChunkSpace(aPosition);

	return GetChunk(pos);
}

vec3i vigame::VoxelWorld::CubeSpaceToChunkSpace(vec3i aPosCubeSpace)
{
	return aPosCubeSpace / Chunk::GetSize();
}

vec3i vigame::VoxelWorld::ChunkSpaceToCubeSpace(vec3i aPosChunkSpace)
{
	return aPosChunkSpace * Chunk::GetSize();
}

vec3 vigame::VoxelWorld::CubeSpaceToWorldSpace(vec3i aPosCubeSpace)
{
	return vec3(aPosCubeSpace) * vec3(mGridSize);
}

void vigame::VoxelWorld::Update(float aDeltaTime)
{
	if (!mGenerateInfinite)
		return;

	for (int z = 0; z < mChunkSize.z; z++)
	{
		for (int y = 0; y < mChunkSize.y; y++)
		{
			for (int x = 0; x < mChunkSize.x; x++)
			{
				vec3i startSearchPos = WorldSpaceToChunkSpace(mLoadPosition) - (WorldSpaceToChunkSpace(mSize) / 2);
				vec3i key = startSearchPos + vec3i(x, y, z);
				if (mChunks.find(key) == mChunks.end())
				{
					MakeChunk(key);
				}
			}
		}
	}
}

void vigame::VoxelWorld::Draw(ViVertexBatch* aBatch)
{
	for (const auto& chunk : mChunks)
	{
		if (chunk.second == nullptr)
			mChunks.erase(chunk.first);
		else chunk.second->Draw(aBatch);
	}

	if (mDrawDebug)
	{
		VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_LESS,
			ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_LINES));
		for (const auto& chunk : mChunks)
		{
				ViTransform trans(vec3(chunk.second->GetWorldPosition()) + vec3(GetGridSize() / 2.f), vec3(0),
					vec3(Chunk::GetSize()) * GetGridSize());
				aBatch->Draw(trans, mCubeMesh);
		}
	}
}

vec3i vigame::VoxelWorld::WorldSpaceToChunkSpace(vec3 aPosition)
{
	return vec3i(aPosition) / Chunk::GetSize();
}

vec3i vigame::VoxelWorld::WorldSpaceToCubeSpace(vec3 aPosition)
{
	return vec3i(aPosition / GetGridSize());
}

vec3i vigame::VoxelWorld::GetChunkRelativePosition(vec3i aChunkPosition)
{
	return aChunkPosition - WorldSpaceToChunkSpace(mLoadPosition);
}
