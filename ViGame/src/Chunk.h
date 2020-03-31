#pragma once

#include <unordered_map>

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
		Chunk(vec3i aPosition, VoxelWorld* aWorld);
		~Chunk();

		static constexpr int cWIDTH = 16;
		static constexpr int cHEIGHT = 16;
		static constexpr int cDEPTH = 16;

		void Draw(ViVertexBatch* aBatch);

		void OptimizeMesh();

		vi_property_named(bool, mDirty, Dirty);

		vi_property_get_named(ViMesh*, mOptimizedMesh, OptimizedMesh);

		vi_property_get_named(vec3i, mPosition, Position);
		vi_property_get_named(VoxelWorld*, mWorld, World);

		vi_property_get_named(ViMesh*, mWireframeMesh, WireframeMesh);

		vi_property_get_named(bool, mHasAnything, HasAnything);
		void NotifyCubeChanged(vec3i aPosition, CubeInstance& aPreviousCubeInstance, CubeInstance& aCubeInstance);

	private:
		ViMesh* mOptimizedMesh;
		ViMesh* mOtherMesh;
		//Mesh for debug mode
		ViMesh* mWireframeMesh;

		vec3i mPosition;
		VoxelWorld* mWorld;

		void GreedyMesh();

		//sstd::unordered_map<cubeid, int> mPresentIds;

		//Gets a cube in cube-space chunk-relative coordinates.
		CubeInstance GetCube(vec3i aPosition);

		bool mHasAnything;
	};
}