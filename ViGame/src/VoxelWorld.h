#pragma once

//TODO make our own map that's more optimized
#include <map>

#include "ViUtil.h"
#include "ViVertexBatch.h"

#include "Cube.h"
#include "CubeInstance.h"
#include "ViGameUtil.h"

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

		void AddCube(Cube* aCube);
		inline cubeid GetId(Cube* aCube);
		inline Cube* GetCube(cubeid aId);

		vi_property_get_named(float, mGridSize, GridSize);

	private:
		vec3i mSize;
		float mGridSize;

		cubeid mCachedCubeid;
		Cube* mCachedCube;
		CubeInstance* mCubes;

#define WIDTH mSize.x
#define HEIGHT mSize.y
#define DEPTH mSize.z

		std::vector<Cube*> mCubeTypes;
		std::map<Cube*, cubeid> mIdCubeMappings;
	};
}