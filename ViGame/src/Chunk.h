#pragma once

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViUtil.h"

class ViVertexBatch;

namespace vigame
{
	class VoxelWorld;

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

	private:
		ViMesh* mOptimizedMesh;

		vec3i mPosition;
		VoxelWorld* mWorld;
	};
}