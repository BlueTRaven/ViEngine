#pragma once

#include <map>

#include "Cube.h"

namespace vigame
{
	class CubeRegistry
	{
	public:
		CubeRegistry();

		void AddCubeType(Cube* aCube);
		cubeid GetCubeId(Cube* aCube);
		Cube* GetCubeType(cubeid aId);

	private:
		cubeid mCachedCubeid;
		Cube* mCachedCube;

		std::vector<Cube*> mCubeTypes;
		std::map<Cube*, cubeid> mIdCubeMappings;

	};
}