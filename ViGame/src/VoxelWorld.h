#pragma once

#include "ViUtil.h"

#include "Cube.h"
#include "CubeInstance.h"
#include "CubeRegistry.h"
#include "Chunk.h"
#include "ViGameUtil.h"
#include "WorldGenerator.h"
#include "ProgramCubesInstanced.h"

class ViVertexBatch;

namespace vigame
{
	class VoxelWorld
	{
	public:
		VoxelWorld(vec3i aSize, float aGridSize, WorldGenerator* aWorldGenerator);

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

		void Update(float aDeltaTime);
		void Draw(ViVertexBatch* batch);

		vi_property_get_named(vec3i, mSize, Size);
		vi_property_get_named(vec3i, mChunkSize, ChunkSize);
		vi_property_get_named(float, mGridSize, GridSize);

		vi_property_get_named(WorldGenerator*, mGenerator, Generator);
		vi_property_get_named(CubeRegistry*, mCubeRegistry, CubeRegistry);

		vi_property_named(bool, mDrawDebug, DrawDebug);

		//The position around which we load chunks
		vi_property_named(vec3, mLoadPosition, LoadPosition);

		template<typename TCallback>
		bool VoxelRaycast(glm::vec3& aStart, glm::vec3& aEnd, vec3i& aOut, TCallback aCallback);

	private:
		vec3i mSize;
		float mGridSize;

		vec3i mChunkSize;

		//1d array of cubes - equal to [WIDTH * HEIGHT * DEPTH]
		CubeInstance voidCube = CubeInstance(0);

		std::unordered_map<vec3i, Chunk*> mChunks;

		CubeRegistry* mCubeRegistry;

		WorldGenerator* mGenerator;

		ProgramCubesInstanced* mProgramCubesInstanced;

		ViMesh* mCubeMesh;

		vec3i WorldSpaceToChunkSpace(vec3 aPosition);
		vec3i WorldSpaceToCubeSpace(vec3 aPosition);
		vec3i GetChunkRelativePosition(vec3i aChunkPosition);

		vec3 mOldLoadPosition;

		float mTimer;

		bool mGenerateInfinite = true;
	};

	template <typename TCallback>
	bool vigame::VoxelWorld::VoxelRaycast(vec3& aStart, vec3& aEnd, vec3i& aOut, TCallback aCallback)
	{
		const float x1 = -aStart.x;
		const float y1 = -aStart.y;
		const float z1 = -aStart.z;
		const float x2 = -aEnd.x;
		const float y2 = -aEnd.y;
		const float z2 = -aEnd.z;

		int i = (int)floorf(x1);
		int j = (int)floorf(y1);
		int k = (int)floorf(z1);

		const int iend = (int)floorf(x2);
		const int jend = (int)floorf(y2);
		const int kend = (int)floorf(z2);

		const int di = ((x1 < x2) ? 1 : ((x1 > x2) ? -1 : 0));
		const int dj = ((y1 < y2) ? 1 : ((y1 > y2) ? -1 : 0));
		const int dk = ((z1 < z2) ? 1 : ((z1 > z2) ? -1 : 0));

		const float deltatx = 1.0f / std::abs(x2 - x1);
		const float deltaty = 1.0f / std::abs(y2 - y1);
		const float deltatz = 1.0f / std::abs(z2 - z1);

		const float minx = floorf(x1), maxx = minx + 1.0f;
		float tx = ((x1 > x2) ? (x1 - minx) : (maxx - x1)) * deltatx;
		const float miny = floorf(y1), maxy = miny + 1.0f;
		float ty = ((y1 > y2) ? (y1 - miny) : (maxy - y1)) * deltaty;
		const float minz = floorf(z1), maxz = minz + 1.0f;
		float tz = ((z1 > z2) ? (z1 - minz) : (maxz - z1)) * deltatz;

		aOut = vec3i(i, j, k);

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