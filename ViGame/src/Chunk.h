#pragma once

#include <chrono>
#include <mutex>
#include <unordered_map>
#include <set>

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViUtil.h"

#include "CubeInstance.h"
#include "ChunkLoader.h"

class ViVertexBatch;

namespace vigame
{
	class VoxelWorld;

	class Chunk
	{
	public:
		enum MeshingMethod
		{
			cSTUPID,	//TODO implement stupid meshing?
			cNAIVE,
			cGREEDY
		};

		enum ChunkState 
		{
			cUNINIT,
			cLOADING,
			cDONE
		};

		//aPosition: position in chunk space. Used to index into world chunk array.
		//aWorld: world that this chunk belongs to.
		Chunk(vec3i aWorldPosition, VoxelWorld* aWorld);
		~Chunk();

		//Set the size, in cubes.
		static void SetSize(vec3i aSize);
		//Get the size, in cubes.
		static vec3i GetSize();

		static void SetMeshingMethod(MeshingMethod aMeshingMethod);
		static MeshingMethod GetMeshingMethod();

		void Draw(ViVertexBatch* aBatch);

		vi_property_get_named(ChunkState, mChunkState, ChunkState);

		vi_property_named(bool, mGenerated, Generated);
		vi_property_named(bool, mDirty, Dirty);

		vi_property_get_named(ViMesh*, mOptimizedMesh, OptimizedMesh);

		vi_property_get_named(vec3i, mWorldPosition, WorldPosition);

		vi_property_get_named(VoxelWorld*, mWorld, World);

		vi_property_get_named(bool, mHasAnything, HasAnything);

		//Gets a cube in cube-space chunk-relative coordinates.
		CubeInstance GetCubeRelative(vec3i aPosition);
		void SetCubeRelative(CubeInstance instance, vec3i aPosition);

		//Gets a cube in relative space, that may potentially be not in this chunk
		CubeInstance GetCubePotentially(vec3i aPosition);

		bool CanDelete();

	private:
		ChunkState mChunkState;

		CubeInstance* mCubes;

		vec3i mWorldPosition;

		VoxelWorld* mWorld;

		ViMesh* mOptimizedMesh;
		ViMesh* mOldOptimizedMesh;

		void MakeMesh(MeshingMethod aMethod);

		ViMesh* GreedyMesh();
		ViMesh* NaiveMesh();

		bool mHasAnything;

		ChunkLoader* mChunkLoader;

		static vec3i mSize;
		
		static MeshingMethod mMeshingMethod;
	};
}