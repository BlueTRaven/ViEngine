#pragma once

#include "glm/glm.hpp"

#include "ViUtil.h"
#include "ViMesh.h"
#include "ViColorsGL.h"

#include "ViGameUtil.h"

namespace vigame
{
	class VoxelWorld;

	class Cube
	{
	public:
		Cube(VoxelWorld* world, ViMaterial* aMaterial);

		vi_property_named(ViMesh*, mMesh, Mesh);

		void SetId(cubeid mId);
		inline cubeid GetId();

	protected:
		inline VoxelWorld* GetWorld();

	private:
		bool idSet;

		cubeid mId;

		VoxelWorld* mWorld;
		void CreateMesh(ViMaterial* aMaterial);
	};
}