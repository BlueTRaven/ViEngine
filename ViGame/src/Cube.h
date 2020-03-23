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
		enum CubeFace
		{
			cFACE_TOP,
			cFACE_BOTTOM,
			cFACE_LEFT,
			cFACE_RIGHT,
			cFACE_FRONT,
			cFACE_BACK
		};

		Cube(VoxelWorld* world, ViMaterial* aMaterial);

		vi_property_named(ViMesh*, mMesh, Mesh);
		vi_property_get_named(bool, mTransparent, Transparent);

		void SetId(cubeid mId);
		cubeid GetId();

		void GetFace(CubeFace aFace, std::vector<ViVertex>& aVertices, std::vector<GLuint>& aIndices);

	protected:
		VoxelWorld* GetWorld();

	private:
		bool idSet;

		cubeid mId;

		bool mTransparent;

		VoxelWorld* mWorld;
		void CreateMesh(ViMaterial* aMaterial);
	};
}