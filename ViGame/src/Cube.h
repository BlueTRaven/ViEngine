#pragma once

#include "glm/glm.hpp"

#include "ViUtil.h"
#include "ViMesh.h"
#include "ViColorsGL.h"

namespace vigame
{
	class VoxelWorld;

	class Cube
	{
	public:
		Cube(VoxelWorld* world, ViMaterial* aMaterial);

		vi_property_named(ViMesh*, mMesh, Mesh);

	protected:
		inline VoxelWorld* GetWorld();

	private:
		VoxelWorld* mWorld;
		void CreateMesh(ViMaterial* aMaterial);
	};
}