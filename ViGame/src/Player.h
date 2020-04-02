#pragma once

#include "ViTransform.h"
#include "ViUtil.h"

namespace vigame
{
	class VoxelWorld;
	class Camera;

	class Player
	{
	public:
		Player(ViTransform aStartTransform, VoxelWorld* aWorld, Camera* aCamera);

		void Update(double aDeltaTime);
		void CollisionDetect();

		vi_property_get_named(VoxelWorld*, mWorld, World);

		vi_property_named(vec3, mVelocity, Velocity);
		vi_property_named(ViTransform, mTransform, Transform);

	private:
		float mMovementSpeed = 1;
		float drag = 0.98f;

		int height;
		int width;

		Camera* mCamera;
		VoxelWorld* mWorld;

		vec3i mHighlightedCube;
		bool mAttackKey;
	};
}