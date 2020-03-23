#pragma once

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViUtil.h"

namespace vigame
{
	class VoxelWorld;

	class Chunk
	{
	public:
		Chunk(vec3i aPosition, VoxelWorld* aWorld);

		const int cWIDTH = 16;
		const int cHEIGHT = 16;
		const int cDEPTH = 16;

		void OptimizeMesh();

		vi_property_get_named(ViMesh*, mOptimizedMesh, OptimizedMesh);

		vi_property_get_named(vec3i, mPosition, Position);
		vi_property_get_named(VoxelWorld*, mWorld, World);

	private:
		ViMesh* mOptimizedMesh;

		vec3i mPosition;
		VoxelWorld* mWorld;
	};
}