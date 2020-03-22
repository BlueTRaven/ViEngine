#pragma once

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViUtil.h"

namespace vigame
{
	class Chunk
	{
	public:
		const int cWIDTH = 16;
		const int cHEIGHT = 16;
		const int cDEPTH = 16;

		void OptimizeMesh();

		vi_property_get_named(ViMesh*, mOptimizedMesh, OptimizedMesh);

	private:
		ViMesh* mOptimizedMesh;
	};
}