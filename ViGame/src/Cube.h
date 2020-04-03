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
		Cube(VoxelWorld* world, ViMaterial* aMaterial, ViColorGL aColor, bool aTransparent);

		vi_property_get_named(bool, mTransparent, Transparent);
		vi_property_get_named(ViColorGL, mColor, Color);
		vi_property_get_named(ViMesh*, mMesh, Mesh);

		void SetId(cubeid mId);
		inline cubeid GetId()
		{
			return mId;
		}

		uint8_t GetAdjacents(const CubeInstance& aCubeInstance, vec3i aPosition);

		std::vector<int> GetSubsectionsToDraw(const CubeInstance& aCubeInstance, vec3i aPosition);
		
		ViTransform GetWorldSpaceTransform(const CubeInstance& aCubeInstance, vec3i aPosition);

		void Draw(const CubeInstance& aCubeInstance, vec3i aPosition, ViVertexBatch* aBatch);

	protected:
		VoxelWorld* GetWorld();

	private:
		bool idSet;

		cubeid mId;

		bool mTransparent;
		ViColorGL mColor;

		VoxelWorld* mWorld;
		void CreateMesh(ViMaterial* aMaterial);

		bool GetAdjacentCubeShouldHideFace(vec3i aPosition);

		ViMaterial* mMaterial;
		ViMesh* mMesh;
	};
}