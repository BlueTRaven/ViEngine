#pragma once

#include <chrono>
#include <mutex>
#include <unordered_map>
#include <set>

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViUtil.h"

class ViVertexBatch;

namespace vigame
{
	class VoxelWorld;
	struct CubeInstance;

	class Chunk
	{
	public:
		//aPosition: position in chunk space. Used to index into world chunk array.
		//aWorld: world that this chunk belongs to.
		Chunk(vec3i aRelativePosition, vec3i aWorldPosition, VoxelWorld* aWorld);
		~Chunk();

		//Set the size, in cubes.
		static void SetSize(vec3i aSize);
		//Get the size, in cubes.
		static vec3i GetSize();

		void Draw(ViVertexBatch* aBatch);

		vi_property_named(bool, mDirty, Dirty);

		vi_property_get_named(ViMesh*, mOptimizedMesh, OptimizedMesh);

		vi_property_set_named(vec3i, mRelativePosition, RelativePosition);
		vi_property_get_named(vec3i, mRelativePosition, RelativePosition);
		vi_property_get_named(vec3i, mWorldPosition, WorldPosition);

		vi_property_get_named(VoxelWorld*, mWorld, World);

		vi_property_get_named(ViMesh*, mWireframeMesh, WireframeMesh);

		vi_property_get_named(bool, mHasAnything, HasAnything);

		//Gets a cube in cube-space chunk-relative coordinates.
		CubeInstance GetCube(vec3i aPosition);
	private:
		enum MeshingMethod
		{
			cSTUPID,	//TODO implement stupid meshing?
			cNAIVE,
			cGREEDY
		};

		ViMesh* mOptimizedMesh;
		ViMesh* mOldOptimizedMesh;
		//Mesh for debug mode
		ViMesh* mWireframeMesh;

		vec3i mRelativePosition;
		vec3i mWorldPosition;

		VoxelWorld* mWorld;

		//Checks to see if we need to mesh, and begins the meshing thread if so
		void TryMeshing(MeshingMethod aMethod);

		void GreedyMesh();
		void NaiveMesh();

		bool meshing = false;
		std::thread* meshingThread;
		std::mutex* mut;

		bool mHasAnything;

		static vec3i mSize;
		
	};
}