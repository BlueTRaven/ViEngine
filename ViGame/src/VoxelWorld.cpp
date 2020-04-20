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

	for (int i = 0; i < (mChunkSize.x * mChunkSize.y * mChunkSize.z); i++)
	{
		vec3i pos = IndexTo3DIndex(i, mChunkSize);
		Chunk* chunk = new Chunk(pos, pos, this);

		mChunks.push_back(chunk);
	}

	//Cube with no mesh
	mCubeRegistry->AddCubeType(new Cube(this, nullptr, vicolors::WHITE, false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("white_pixel"), vicolors::WHITE, false));
	mCubeRegistry->AddCubeType(new Cube(this, GET_ASSET_MATERIAL("white_pixel"), vicolors::GREEN, true));

	mGenerator->Init(this);
	for (int i = 0; i < (mChunkSize.x * mChunkSize.y * mChunkSize.z); i++)
	{
		vec3i pos = IndexTo3DIndex(i, mChunkSize);
		mGenerator->GenerateChunk(pos);
	}

	mCubeMesh = ViMesh::MakeUCube(ASSET_HANDLER->LoadMaterial("white_pixel_fullbright"), vec3(-0.5), vec3(0.5), ViMesh::cFACE_ALL, vicolors::WHITE);
	mCubeMesh->UploadData();
}

void vigame::VoxelWorld::SetCube(vec3i aPosition, Cube* aCube)
{
	Chunk* chunk = GetChunkResponsibleForCube(aPosition);

	if (chunk == nullptr)
		return;

	chunk->SetCubeRelative(MakeInstance(aCube), aPosition - chunk->GetWorldPosition() * Chunk::GetSize());
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

vigame::Chunk* vigame::VoxelWorld::GetChunk(vec3i aChunkPosition)
{
	if (aChunkPosition.x < 0 || aChunkPosition.y < 0 || aChunkPosition.z < 0 || aChunkPosition.x > mSize.x || aChunkPosition.y > mSize.y || aChunkPosition.z > mSize.z)
		return nullptr;

	return mChunks[Vec3IndexToIndex(aChunkPosition, mChunkSize)];
}

vigame::Chunk* vigame::VoxelWorld::MakeChunk(vec3i aChunkRelativePosition, vec3i aChunkWorldPosition)
{
	Chunk* chunk = new Chunk(aChunkRelativePosition, aChunkWorldPosition, this);
	mChunks[Vec3IndexToIndex(aChunkRelativePosition, mChunkSize)] = chunk;

	mGenerator->GenerateChunk(aChunkWorldPosition);
	return chunk;
}

void vigame::VoxelWorld::RemoveChunk(vec3i aChunkPosition)
{
	delete GetChunk(aChunkPosition);
}

vigame::Chunk* vigame::VoxelWorld::MoveChunk(vec3i aOldChunkPos, vec3i aNewChunkPos)
{
	Chunk* oldChunk = GetChunk(aOldChunkPos);
	Chunk* newChunk = GetChunk(aNewChunkPos);
	mChunks[Vec3IndexToIndex(aNewChunkPos, mChunkSize)] = oldChunk;

	return newChunk;
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
	mTimer += aDeltaTime;

	return;

	vec3i oldChunkLoadPos = WorldSpaceToChunkSpace(mOldLoadPosition);
	vec3i chunkLoadPos = WorldSpaceToChunkSpace(mLoadPosition);

	vec3i diff = chunkLoadPos - oldChunkLoadPos;

	if (diff.x > 0)
	{
		for (int z = 0; z < mChunkSize.z; z++)
		{
			for (int y = 0; y < mChunkSize.y; y++)
			{
				int amtX = glm::min(diff.x, mChunkSize.x);
				for (int x = 0; x < amtX; x++)
				{
					RemoveChunk(vec3i(x, y, z));
				}

				//if amtX == mChunkSize.x - 1, the whole world has been removed. We don't need to bother shifting things around.
				/*if (amtX != mChunkSize.x - 1)
				{
					for (int x = amtX; x < mChunkSize.x; x++)
					{
						MoveChunk(vec3i(x, y, z), vec3i(x - 1, y, z));
					}
				}*/

				//Generate what we removed.
				for (int x = 0; x < amtX; x++)
				{
					vec3i removedPos = vec3i(x, y, z);
					
					MakeChunk(removedPos, removedPos + vec3i(chunkLoadPos.x, 0, 0));
				}
			}
		}
	}
	else if (diff.x < 0)
	{
		//Remove last column
		//Shift columns right one
		//Generate first column
	}
	mOldLoadPosition = mLoadPosition;
}

void vigame::VoxelWorld::Draw(ViVertexBatch* aBatch)
{
	for (int i = 0; i < mChunks.size(); i++)
	{
		if (mChunks[i] != nullptr)	//BANDAID nocheckin
			mChunks[i]->Draw(aBatch);
	}

	if (mDrawDebug)
	{
		VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_LESS,
			ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_LINES));
		for (int i = 0; i < mChunks.size(); i++)
		{
			if (mChunks[i] != nullptr)
			{
				ViTransform trans(vec3(mChunks[i]->GetWorldPosition()) + vec3(GetGridSize() / 2.f), vec3(0),
					vec3(Chunk::GetSize()) * GetGridSize());
				aBatch->Draw(trans, mCubeMesh);
			}
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
