#pragma once

//TODO make our own map that's more optimized
#include <map>

#include "ViUtil.h"

#include "Cube.h"
#include "CubeInstance.h"
#include "ViVertexBatch.h"

namespace vigame
{
	class VoxelWorldGenerator
	{
	public:
		VoxelWorldGenerator(vec3i aSize);

		void SetCubeInstance(vec3i aPosition, Cube* aCube);
		CubeInstance& GetCubeInstance(vec3i aPosition);

		CubeInstance MakeInstance(Cube* aCube);

		void Draw(ViVertexBatch* batch);

	private:
		vec3i mSize;

		CubeInstance* mCubes;

#define WIDTH mSize.x
#define HEIGHT mSize.y
#define DEPTH mSize.z

		void AddCube(Cube* aCube, cubeid aId);
		cubeid GetId(Cube* aCube);
		Cube* GetCube(cubeid aId);

		std::map<cubeid, Cube*> mCubeIdMappings;
		std::map<Cube*, cubeid> mIdCubeMappings;
	};
}