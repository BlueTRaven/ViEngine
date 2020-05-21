#include "VoxelWorld.h"

#include "ViEnvironment.h"
#include "ViVertexBatch.h"
#include "ViFrameBuffer.h"

#define STB_DS_IMPLEMENTATION
#include "stb/stb_ds.h"
#undef STB_DS_IMPLEMENTATION

vigame::VoxelWorld::VoxelWorld(vec3i aSize, float aGridSize, WorldGenerator* aWorldGenerator) :
	mChunks(nullptr),
	mSize(aSize),
	mCubeRegistry(new CubeRegistry()),
	mGridSize(aGridSize),
	mGenerator(aWorldGenerator),
	mChunkManager(new ChunkManager()),
	mDrawDebug(false),
	mLoadPosition(vec3(0)),
	mCubeMesh(nullptr),
	mSkyboxMesh(nullptr),
	mSunMesh(nullptr),
	mMoonMesh(nullptr),
	mTimeOfDay(0),
	mChunksAccessMutex(new std::mutex)
{
	ChunkMap defaultMapObj;	//use default ctor
	hmdefaults(mChunks, defaultMapObj);

	mProgramLitGeneric = static_cast<ProgramLitGeneric*>(GET_ASSET_PROGRAM("lit_generic"));
	mProgramUnlitGeneric = static_cast<ProgramUnlitGeneric*>(GET_ASSET_PROGRAM("unlit_generic"));

	vec3i chunkSize = Chunk::GetSize();

	vi_assert((aSize.x % chunkSize.x) == 0, "Size of world must be divisible by Chunk Width.");
	vi_assert((aSize.y % chunkSize.y) == 0, "Size of world must be divisible by Chunk Height.");
	vi_assert((aSize.z % chunkSize.z) == 0, "Size of world must be divisible by Chunk Depth.");

	mChunkSize = aSize / chunkSize;

	//Cube with no mesh
	mCubeRegistry->AddCubeType(new Cube(this, vicolors::WHITE, false));
	mCubeRegistry->AddCubeType(new Cube(this, vicolors::WHITE, false));
	mCubeRegistry->AddCubeType(new Cube(this, ViColorGL(0.25, 1, 0.25, 1), false));
	mCubeRegistry->AddCubeType(new Cube(this, vicolors::BLUE, true));

	mCubeMesh = ViMesh::MakeUCube(vec3(-0.5), vec3(0.5), ViMesh::cFACE_ALL, vicolors::WHITE);

	//Note positions are inverted so it faces inward
	float skyboxDistance = 512;
	mSkyboxMesh = ViMesh::MakeUCube(vec3(skyboxDistance), vec3(-skyboxDistance), ViMesh::cFACE_ALL, vicolors::WHITE);

	float sunSize = 8;
	float moonSize = 5;
	mSunMesh = ViMesh::MakeUCube(vec3(-sunSize), vec3(sunSize), ViMesh::cFACE_ALL, vicolors::YELLOW);
	mMoonMesh = ViMesh::MakeUCube(vec3(-moonSize), vec3(moonSize), ViMesh::cFACE_ALL, ViColorGL(0.88, 0.88, 0.95, 1.0));

	mWorldFrameBuffer = new ViFrameBuffer(ENVIRONMENT->GetScreenWidth(), ENVIRONMENT->GetScreenHeight(), ViFrameBuffer::cCOLOR_ALL, ViFrameBuffer::cDEPTH_READ);

	mTestFontMat = new ViMaterialFont(GET_ASSET_FONT("debug"), GET_ASSET_PROGRAM("text"));
}

void vigame::VoxelWorld::Init()
{
	mGenerator->Init(this);

	for (int z = -mViewDistanceChunks.z; z <= mViewDistanceChunks.z; z++)
	{
		for (int y = -mViewDistanceChunks.y; y <= mViewDistanceChunks.y; y++)
		{
			for (int x = -mViewDistanceChunks.x; x <= mViewDistanceChunks.x; x++)
			{
				vec3i key = WorldSpaceToChunkSpace(mLoadPosition) + vec3i(x, y, z);

				MakeChunk(key);
			}
		}
	}

	mChunkManager->SortChunksByDistance(WorldSpaceToChunkSpace(mLoadPosition));
	mChunkManager->Start();

	mCubeMesh->UploadData();
}

void vigame::VoxelWorld::SetCube(vec3i aPosition, Cube* aCube)
{
	Chunk* chunk = GetChunkResponsibleForCube(aPosition);

	if (chunk == nullptr)
		return;

	vec3i relPosition = RoundToVec3i(vec3(aPosition) - vec3(chunk->GetWorldPosition() * Chunk::GetSize()));
	chunk->SetCubeRelative(MakeInstance(aCube->GetId()), relPosition);
}

vigame::CubeInstance& vigame::VoxelWorld::GetCube(vec3i aPosition)
{	
	Chunk* chunk = GetChunkResponsibleForCube(aPosition);

	if (chunk == nullptr)
		return voidCube;

	vec3i relPosition = RoundToVec3i(vec3(aPosition) - vec3(chunk->GetWorldPosition() * Chunk::GetSize()));
	return chunk->GetCubeRelative(relPosition);
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
	mChunksAccessMutex->lock();
	if (hmgets(mChunks, aChunkPosition).state == ChunkMap::cNONE)
	{
		//DON'T FORGET TO UNLOCK WHEN RETURNING EARLY...
		mChunksAccessMutex->unlock();
		return nullptr;
	}

	Chunk* chunk = hmgets(mChunks, aChunkPosition).value; //mChunks[aChunkPosition];
	mChunksAccessMutex->unlock();

	return chunk;
}

vigame::Chunk* vigame::VoxelWorld::MakeChunk(vec3i aChunkWorldPosition)
{
	Chunk* chunk = new Chunk(aChunkWorldPosition, this);

	mChunkManager->AddChunkToLoad(chunk);

	mChunksAccessMutex->lock();
	//add a dummy value so we don't try to load again

	ChunkMap obj;
	obj.key = aChunkWorldPosition;
	obj.value = nullptr;
	obj.state = ChunkMap::cLOADING;
	hmputs(mChunks, obj);
	mChunksAccessMutex->unlock();

	return chunk;
}

void vigame::VoxelWorld::RemoveChunk(vec3i aChunkPosition)
{
	delete GetChunk(aChunkPosition);
	hmdel(mChunks, aChunkPosition);	//TODO does this delete the chunk too?
	//mChunks.erase(aChunkPosition);
}

void vigame::VoxelWorld::Update(double aDeltaTime)
{
	if (!mGenerateInfinite)
		return;

	for (int z = -mViewDistanceChunks.z; z <= mViewDistanceChunks.z; z++)
	{
		for (int y = -mViewDistanceChunks.y; y <= mViewDistanceChunks.y; y++)
		{
			for (int x = -mViewDistanceChunks.x; x <= mViewDistanceChunks.x; x++)
			{
				vec3i key = WorldSpaceToChunkSpace(mLoadPosition) + vec3i(x, y, z);

				auto out = hmgets(mChunks, key);
				if (out.state == ChunkMap::cNONE)// mChunks.find(key) == mChunks.end())
				{
					MakeChunk(key);
				}
			}
		}
	}

	mChunkManager->SortChunksByDistance(WorldSpaceToChunkSpace(mLoadPosition));

	std::vector<Chunk*> loadedChunks = mChunkManager->GetFinishedChunks();

	for (Chunk* chunk : loadedChunks)
	{
		//replace dummy chunk
		ChunkMap obj;
		obj.key = chunk->GetWorldPosition();
		obj.value = chunk;
		obj.state = ChunkMap::cDONE;
		hmputs(mChunks, obj);
		//mChunks[chunk->GetWorldPosition()] = chunk;
		chunk->LoadFinished();
	}

	//We use a unordered_map in order to store our chunks, in order to make them accessible at O(1) by their positions in chunk space.
	//however, unordered_map is... painfully slow. Even if we get a better implementation, it's still going to be slower than a flat array.
	//So, we instead cache the chunks once they've finished loading, and only re-cache them when we move.
	if (mHasChunksToCache || WorldSpaceToChunkSpace(mLoadPosition) != WorldSpaceToChunkSpace(mOldLoadPosition))
	{
		mHasChunksToCache = false;
		mCachedDrawChunks.clear();

		for (int z = -mViewDistanceChunks.z; z <= mViewDistanceChunks.z; z++)
		{
			for (int y = -mViewDistanceChunks.y; y <= mViewDistanceChunks.y; y++)
			{
				for (int x = -mViewDistanceChunks.x; x <= mViewDistanceChunks.x; x++)
				{
					vec3i pos = WorldSpaceToChunkSpace(mLoadPosition) + vec3i(x, y, z);
					Chunk* chunk = GetChunk(pos);

					if (chunk != nullptr)
						mCachedDrawChunks.push_back(chunk);
					else mHasChunksToCache = true;	//while any of our chunks are null, we want to keep trying to cache.
				}
			}
		}
	}

	loadedChunks.clear();

	if (INPUT_MANAGER->KeyDown(SDL_SCANCODE_K))
	{
		mWorldFrameBuffer->GetTexture()->WritePNG("./output_color.png");
		mWorldFrameBuffer->GetDepthTexture()->WritePNG("./output_depth.png");
	}
	//Update Sun
	mTimeOfDay += aDeltaTime;
	mTimeOfDay = glm::mod(mTimeOfDay, mEndOfDay);

	mOldLoadPosition = mLoadPosition;
}

void vigame::VoxelWorld::Draw(double aDeltaTime, ViVertexBatch* aBatch)
{
	aBatch->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_LESS,
		ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_FILLED));
	aBatch->SetTarget(mWorldFrameBuffer);
	aBatch->Clear(true, true);

	mProgramUnlitGeneric->SetTintColor(GetRadialFogColor());
	aBatch->Draw(ViTransform::Positioned(mLoadPosition), mSkyboxMesh, GET_ASSET_PROGRAM("unlit_generic"), GET_ASSET_TEXTURE("white_pixel"), 0);
	aBatch->Flush();
	mProgramUnlitGeneric->SetTintColor(vec3(1));
	aBatch->Flush();

	aBatch->Draw(ViTransform::Positioned(mLoadPosition + GetSunPos()), mSunMesh, GET_ASSET_PROGRAM("unlit_generic"), GET_ASSET_TEXTURE("white_pixel"), 0);
	aBatch->Draw(ViTransform::Positioned(mLoadPosition - GetSunPos()), mMoonMesh, GET_ASSET_PROGRAM("unlit_generic"), GET_ASSET_TEXTURE("white_pixel"), 0);

	for (const auto& chunk : mCachedDrawChunks)
	{
		chunk->Draw(aBatch);
	}

	aBatch->SetTarget(nullptr);
	aBatch->Clear(true, true);
	aBatch->DrawQuad(ViTransform::None, vec3(0, 0, -0.1), vec3(ENVIRONMENT->GetScreenWidth(), ENVIRONMENT->GetScreenHeight(), 0), GET_ASSET_PROGRAM("ortho"), mWorldFrameBuffer->GetTexture(), 0);
	aBatch->Flush();

	vec3i chunkPos = WorldSpaceToChunkSpace(mLoadPosition);
	VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_CW, ViVertexBatchSettings::cDEPTH_NONE,
		ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_ALPHABLEND, ViVertexBatchSettings::cDRAW_FILLED));

	aBatch->DrawString(ViTransform::Positioned({ 0, 128 + 24, 0 }), mTestFontMat, 
		"Current delta time: " + std::to_string(aDeltaTime) + "\n");

	if (mDrawDebug)
	{
		VERTEX_BATCH->SetSettings(ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_LESS,
			ViVertexBatchSettings::cCLAMP_POINT, ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED, ViVertexBatchSettings::cDRAW_LINES));
		for (int i = 0; i < hmlen(mChunks); i++)
		{
			Chunk* chunk = mChunks[i].value;
			if (chunk == nullptr)
				continue;

			ViTransform trans(vec3(chunk->GetWorldPosition()) + vec3(GetGridSize() / 2.f), vec3(0),
				vec3(Chunk::GetSize()) * GetGridSize());
			aBatch->Draw(trans, mCubeMesh, GET_ASSET_PROGRAM("unlit_generic"), GET_ASSET_TEXTURE("white_pixel"), 0);
		}
	}
}

vigame::Chunk* vigame::VoxelWorld::GetChunkResponsibleForCube(vec3i aPosition)
{
	vec3i pos = CubeSpaceToChunkSpace(aPosition);

	return GetChunk(pos);
}

vec3i vigame::VoxelWorld::CubeSpaceToChunkSpace(vec3i aPosCubeSpace)
{
	return RoundToVec3i(vec3(aPosCubeSpace) / vec3(Chunk::GetSize()));
}

vec3i vigame::VoxelWorld::ChunkSpaceToCubeSpace(vec3i aPosChunkSpace)
{
	return aPosChunkSpace * Chunk::GetSize();
}

vec3 vigame::VoxelWorld::CubeSpaceToWorldSpace(vec3i aPosCubeSpace)
{
	return vec3(aPosCubeSpace) * vec3(mGridSize);
}

vec3i vigame::VoxelWorld::WorldSpaceToChunkSpace(vec3 aPosition)
{
	return RoundToVec3i(aPosition / vec3(Chunk::GetSize()));
}

vec3i vigame::VoxelWorld::WorldSpaceToCubeSpace(vec3 aPosition)
{
	return RoundToVec3i(aPosition / GetGridSize());
}

vec3i vigame::VoxelWorld::GetChunkRelativePosition(vec3i aChunkPosition)
{
	return aChunkPosition - WorldSpaceToChunkSpace(mLoadPosition);
}

vec3 vigame::VoxelWorld::GetSunPos()
{
	float timePercent = mTimeOfDay / mEndOfDay;

	float x = glm::cos((glm::two_pi<double>()) * timePercent) * 320;
	float y = glm::sin((glm::two_pi<double>()) * timePercent) * 320;
	return vec3(x, y, 0);
}

vec3 vigame::VoxelWorld::GetRadialFogColor()
{
	float startOffset = mEndOfDay * 0.08;
	float dawnStart = mEndOfDay - startOffset;
	float dawnEnd = mEndOfDay * 0.16;

	float duskStart = (mEndOfDay / 2) - (mEndOfDay * 0.08);
	float duskEnd = (mEndOfDay / 2) + (mEndOfDay * 0.16);

	vec3 dayColor = vicolors::BLUE.ToVec4();
	vec3 nightColor = ViColorGL(0.0, 0.01, 0.06, 1.0).ToVec4();

	vec3 col = dayColor;

	if (mTimeOfDay > duskEnd && mTimeOfDay <= dawnStart)
		col = nightColor;
	else if (mTimeOfDay > duskStart && mTimeOfDay <= duskEnd)
	{
		float percent = (mTimeOfDay - duskStart) / (duskEnd - duskStart);
		col = glm::mix(dayColor, nightColor, percent);
	}
	else if ((mTimeOfDay > dawnStart && mTimeOfDay <= mEndOfDay) || (mTimeOfDay >= 0 && mTimeOfDay <= dawnEnd))
	{
		float percent = 0;
		if (mTimeOfDay > dawnStart)
			percent = (mTimeOfDay - dawnStart) / ((mTimeOfDay + dawnEnd) - dawnStart);
		else if (mTimeOfDay <= dawnEnd)
			percent = (mTimeOfDay - (-startOffset)) / dawnEnd;

		col = glm::mix(nightColor, dayColor, percent);
	}

	return col;
}
