#pragma once

#include "ViTransform.h"
#include "ViUtil.h"

namespace vigame
{
	class VoxelWorld;

	class Player
	{
	public:
		Player(VoxelWorld* aWorld);

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

		VoxelWorld* mWorld;
	};
}