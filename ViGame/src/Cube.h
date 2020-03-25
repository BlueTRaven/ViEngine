#pragma once

#include <vector>

#include "glm/glm.hpp"

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
		Cube(VoxelWorld* world, ViMaterial* aMaterial, bool aTransparent);

		vi_property_get_named(bool, mTransparent, Transparent);

		void SetId(cubeid mId);
		cubeid GetId();

		uint8_t GetAdjacents(const CubeInstance& aCubeInstance, vec3i aPosition);

		//ViMesh* GetMeshWithFace(uint8_t aFaces);

		void Draw(const CubeInstance& aCubeInstance, vec3i aPosition, ViVertexBatch* aBatch);

	protected:
		VoxelWorld* GetWorld();

	private:
		bool idSet;

		cubeid mId;

		bool mTransparent;

		VoxelWorld* mWorld;
		void CreateMesh(ViMaterial* aMaterial);

		bool GetAdjacentCubeShouldHideFace(vec3i aPosition);

		ViMaterial* mMaterial;
		ViMesh* mMesh;
	};
}