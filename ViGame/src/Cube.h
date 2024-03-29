#pragma once

#include <vector>

#include "glm/glm.hpp"

#include "ViTransform.h"
#include "ViUtil.h"
#include "ViMesh.h"
#include "ViColorsGL.h"

#include "ViGameUtil.h"

class ViVertexBatch;

namespace vigame
{
	class VoxelWorld;
	struct CubeInstance;

	class Cube
	{
	public:
		Cube(VoxelWorld* world, ViColorGL aColor, bool aTransparent);

		vi_property_get_named(bool, mTransparent, Transparent);
		vi_property_get_named(ViColorGL, mColor, Color);

		void SetId(cubeid mId);
		inline cubeid GetId()
		{
			return mId;
		}

		int GetAdjacents(const CubeInstance& aCubeInstance, vec3i aPosition);

		std::vector<int> GetSubsectionsToDraw(const CubeInstance& aCubeInstance, vec3i aPosition);
		
		ViTransform GetWorldSpaceTransform(const CubeInstance& aCubeInstance, vec3i aPosition);

	protected:
		VoxelWorld* GetWorld();

	private:
		bool idSet;

		cubeid mId;

		bool mTransparent;
		ViColorGL mColor;

		VoxelWorld* mWorld;

		bool GetAdjacentCubeShouldHideFace(vec3i aPosition);
	};
}