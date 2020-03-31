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

		void SetCubeInstance(vec3i aPosition, Cube* aCube);
		CubeInstance& GetCubeInstance(vec3i aPosition);
		CubeInstance MakeInstance(Cube* aCube);

		//Gets the chunk in chunk space. (i.e. uses aChunkPosition to index directly into the mChunks array.)
		Chunk* GetChunk(vec3i aChunkPosition);
		Chunk* MakeChunk(vec3i aChunkPosition);

		Chunk* GetChunkResponsibleForCube(vec3i aPosition);
		//converts from cube space to chunk space. Equivalent to aPosInCubeSpace / mChunkSize.
		vec3i CubeSpaceToChunkSpace(vec3i aPosCubeSpace);
		vec3i ChunkSpaceToCubeSpace(vec3i aPosChunkSpace);

		vec3 CubeSpaceToWorldSpace(vec3i aPosCubeSpace);

		void Draw(ViVertexBatch* batch);

		vi_property_get_named(vec3i, mSize, Size);
		vi_property_get_named(vec3i, mChunkSize, ChunkSize);
		vi_property_get_named(float, mGridSize, GridSize);
		vi_property_get_named(CubeRegistry*, mCubeRegistry, CubeRegistry);

		vi_property_named(bool, mDrawDebug, DrawDebug);
	private:
		vec3i mSize;
		float mGridSize;

		vec3i mChunkSize;

		//1d array of cubes - equal to [WIDTH * HEIGHT * DEPTH]
		CubeInstance* mCubes;
		CubeInstance voidCube = CubeInstance(0);

		//3d array of chunks
		std::vector<Chunk*> mChunksA;
		//std::vector<std::vector<std::vector<Chunk*>>> mChunks;

		CubeRegistry* mCubeRegistry;

		WorldGenerator* mGenerator;

		ProgramCubesInstanced* mProgramCubesInstanced;
	};
}