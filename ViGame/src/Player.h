#pragma once

#include <string>

#include "ViTransform.h"
#include "ViUtil.h"
#include "ViMesh.h"
#include "ViMaterialFont.h"

class ViVertexBatch;

namespace vigame
{
	class VoxelWorld;
	class Camera;

	class Player
	{
	public:
		Player(ViTransform aStartTransform, VoxelWorld* aWorld, Camera* aCamera);

		void Update(double aDeltaTime);
		void Draw(ViVertexBatch* aBatch);
		void CollisionDetect();

		vi_property_get_named(VoxelWorld*, mWorld, World);

		vi_property_named(vec3, mVelocity, Velocity);
		vi_property_named(ViTransform, mTransform, Transform);

		void SetPosition(vec3 aPosition);

	private:
		void Move(double aDeltaTime);

		float mMovementSpeed = 1;
		float mDrag = 0.98f;

		int height;
		int width;

		Camera* mCamera;
		VoxelWorld* mWorld;

		vec3i mHighlightedCube;
		bool mHighlighted;

		bool mNoclip;

		ViMesh* mCubeMesh;

		ViMaterialFont* mMaterialFont;

		vec3 mMaxVelocity;
		float mAcceleration;
	};
}