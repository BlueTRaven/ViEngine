#pragma once

#include <mutex>

#include "ViUtil.h"
#include "ViMaterialFont.h"

#include "Cube.h"
#include "CubeInstance.h"
#include "CubeRegistry.h"
#include "Chunk.h"
#include "ViGameUtil.h"
#include "WorldGenerator.h"
#include "ProgramLitGeneric.h"
#include "ProgramUnlitGeneric.h"
#include "ProgramShadowmap.h"
#include "ChunkManager.h"

class ViVertexBatch;
class ViFrameBuffer;
class ViTextureDrawInstance;

namespace vigame
{
	struct ChunkMap
	{
		enum ChunkMapState
		{
			cNONE,
			cLOADING,
			cDONE
		};

		vec3i key = vec3i(0);
		Chunk* value = nullptr;
		ChunkMapState state = cNONE;
	};

	class VoxelWorld
	{
	public:
		//using ChunkMap = std::unordered_map<vec3i, Chunk*>;

		VoxelWorld(vec3i aSize, float aGridSize, WorldGenerator* aWorldGenerator);

		void Init();

		void SetCube(vec3i aPosition, Cube* aCube);
		CubeInstance& GetCube(vec3i aPosition);
		CubeInstance MakeInstance(Cube* aCube);
		CubeInstance MakeInstance(cubeid aId);

		//Gets the chunk in chunk space. (i.e. uses aChunkPosition to index directly into the mChunks array.)
		Chunk* GetChunk(vec3i aChunkPosition);
		Chunk* MakeChunk(vec3i aChunkWorldPosition);
		void RemoveChunk(vec3i aChunkPosition);

		Chunk* GetChunkResponsibleForCube(vec3i aPosition);
		//converts from cube space to chunk space. Equivalent to aPosInCubeSpace / mChunkSize.
		vec3i CubeSpaceToChunkSpace(vec3i aPosCubeSpace);
		vec3i ChunkSpaceToCubeSpace(vec3i aPosChunkSpace);

		vec3 CubeSpaceToWorldSpace(vec3i aPosCubeSpace);

		vec3i WorldSpaceToChunkSpace(vec3 aPosition);
		vec3i WorldSpaceToCubeSpace(vec3 aPosition);

		inline bool PointWithinBounds(vec3 aPoint, vec3i aCube)
		{
			return WorldSpaceToCubeSpace(aPoint) == aCube;
		}

		void Update(double aDeltaTime);
		void Draw(double aDeltaTime,  ViVertexBatch* batch);

		vi_property_get_named(vec3i, mSize, Size);
		vi_property_get_named(vec3i, mChunkSize, ChunkSize);
		vi_property_get_named(float, mGridSize, GridSize);

		vi_property_get_named(WorldGenerator*, mGenerator, Generator);
		vi_property_get_named(ChunkManager*, mChunkManager, ChunkManager);
		vi_property_get_named(CubeRegistry*, mCubeRegistry, CubeRegistry);

		vi_property_named(vec3i, mViewDistanceChunks, ViewDistanceChunks);
		vi_property_named(bool, mDrawDebug, DrawDebug);

		//The position around which we load chunks
		vi_property_named(vec3, mLoadPosition, LoadPosition);

		template<typename TCallback>
		bool VoxelRaycast(glm::vec3& aStart, glm::vec3& aEnd, vec3i& aOut, TCallback aCallback);

		vec3 GetRadialFogColor();
		vec3 GetSunPos();

	private:
		vec3i mSize;
		float mGridSize;

		vec3i mChunkSize;

		vec3 mOldLoadPosition;
		std::vector<Chunk*> mCachedDrawChunks;
		bool mHasChunksToCache = true;

		ViMaterialFont* mTestFontMat;

		CubeInstance voidCube = CubeInstance(0);

		ChunkMap* mChunks;
		std::mutex* mChunksAccessMutex;

		std::vector<Chunk*> mOldChunks;

		CubeRegistry* mCubeRegistry;

		WorldGenerator* mGenerator;
		ChunkManager* mChunkManager;

		ProgramLitGeneric* mProgramLitGeneric;
		ProgramUnlitGeneric* mProgramUnlitGeneric;
		ProgramShadowmap* mProgramShadowmap;

		vec3i GetChunkRelativePosition(vec3i aChunkPosition);

		double mTimeOfDay;
		const double mEndOfDay = 5;

		bool mGenerateInfinite = true;

		ViMesh* mCubeMesh;
		ViMesh* mSkyboxMesh;
		ViMesh* mSunMesh;
		ViMesh* mMoonMesh;

		ViFrameBuffer* mShadowMapFrameBuffer;

		//everything in the world is drawn to this framebuffer
		ViFrameBuffer* mWorldFrameBuffer;

		//Infinite generation needs special rounding
		inline vec3i RoundToVec3i(vec3 aPosition)
		{
			aPosition.x = floorf(aPosition.x);
			aPosition.y = floorf(aPosition.y);
			aPosition.z = floorf(aPosition.z);

			return aPosition;
		}

		inline int RoundToInt(float aValue)
		{
			if (aValue < 0)
				return floorf(aValue);
			else return floorf(aValue);
		}
	};

	template <typename TCallback>
	bool vigame::VoxelWorld::VoxelRaycast(vec3& aStart, vec3& aEnd, vec3i& aOut, TCallback aCallback)
	{
		const float x1 = aStart.x;
		const float y1 = aStart.y;
		const float z1 = aStart.z;
		const float x2 = aEnd.x;
		const float y2 = aEnd.y;
		const float z2 = aEnd.z;

		int i = (int)RoundToInt(x1);
		int j = (int)RoundToInt(y1);
		int k = (int)RoundToInt(z1);

		const int iend = (int)RoundToInt(x2);
		const int jend = (int)RoundToInt(y2);
		const int kend = (int)RoundToInt(z2);

		const int di = ((x1 < x2) ? 1 : ((x1 > x2) ? -1 : 0));
		const int dj = ((y1 < y2) ? 1 : ((y1 > y2) ? -1 : 0));
		const int dk = ((z1 < z2) ? 1 : ((z1 > z2) ? -1 : 0));

		const float deltatx = 1.0f / std::abs(x2 - x1);
		const float deltaty = 1.0f / std::abs(y2 - y1);
		const float deltatz = 1.0f / std::abs(z2 - z1);

		const float minx = RoundToInt(x1), maxx = minx + 1.0f;
		float tx = ((x1 > x2) ? (x1 - minx) : (maxx - x1)) * deltatx;
		const float miny = RoundToInt(y1), maxy = miny + 1.0f;
		float ty = ((y1 > y2) ? (y1 - miny) : (maxy - y1)) * deltaty;
		const float minz = RoundToInt(z1), maxz = minz + 1.0f;
		float tz = ((z1 > z2) ? (z1 - minz) : (maxz - z1)) * deltatz;

		aOut = RoundToVec3i(vec3(x1, y1, z1));

		for (;;)
		{
			if (aCallback(aOut))
				return false;

			if (tx <= ty && tx <= tz)
			{
				if (i == iend) break;
				tx += deltatx;
				i += di;

				if (di == 1) aOut.x++;
				if (di == -1) aOut.x--;
			}
			else if (ty <= tz)
			{
				if (j == jend) break;
				ty += deltaty;
				j += dj;

				if (dj == 1) aOut.y++;
				if (dj == -1) aOut.y--;
			}
			else
			{
				if (k == kend) break;
				tz += deltatz;
				k += dk;

				if (dk == 1) aOut.z++;
				if (dk == -1) aOut.z--;
			}
		}

		return true;
	}
}