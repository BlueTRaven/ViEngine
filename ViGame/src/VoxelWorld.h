#pragma once

//TODO make our own map that's more optimized
#include <map>

#include "ViUtil.h"

#include "Cube.h"
#include "CubeInstance.h"
#include "CubeRegistry.h"
#include "ViGameUtil.h"

class ViVertexBatch;

namespace vigame
{
	class VoxelWorld
	{
	public:
		VoxelWorld(vec3i aSize, float aGridSize);

		void SetCubeInstance(vec3i aPosition, Cube* aCube);
		inline CubeInstance& GetCubeInstance(vec3i aPosition);

		CubeInstance MakeInstance(Cube* aCube);

		void Draw(ViVertexBatch* batch);

		vi_property_get_named(float, mGridSize, GridSize);
		vi_property_get_named(CubeRegistry*, mCubeRegistry, CubeRegistry);

	private:
		vec3i mSize;
		float mGridSize;

		//1d array of cubes - equal to [WIDTH * HEIGHT * DEPTH]
		CubeInstance* mCubes;

		CubeRegistry* mCubeRegistry;

#define WIDTH mSize.x
#define HEIGHT mSize.y
#define DEPTH mSize.z
	};
}