#pragma once

#include <random>
#include <stdint.h>

#include "ViUtil.h"

namespace vigame
{
	typedef uint8_t cubeid;	//define a type for cube ids separately since this can change later on

	inline vec3i IndexTo3DIndex(int aIndex, vec3i aStrides)
	{
		int z = aIndex % aStrides.z;
		int y = (aIndex / aStrides.z) % aStrides.y;
		int x = aIndex / (aStrides.y * aStrides.z);

		return { x, y, z };
	}

	inline int Vec3IndexToIndex(vec3i aIndex, vec3i aStrides)
	{
		return aIndex.z + aIndex.y * aStrides.z + aIndex.x * aStrides.z * aStrides.y;
	}

	inline int Vec2IndexToIndex(vec2i aIndex, vec2i aStrides)
	{
		return aIndex.x + aStrides.x * aIndex.y;
	}

	inline int RandomInt(std::random_device* rand, int aStart, int aEnd)
	{
		std::default_random_engine engine((*rand)());
		std::uniform_int_distribution distribution(aStart, aEnd);
		return distribution(engine);
	}

	inline float RandomFloat(std::random_device* rand, float aStart, float aEnd)
	{
		std::default_random_engine engine((*rand)());
		std::uniform_real_distribution<float> distribution(aStart, aEnd);
		return distribution(engine);
	}

	inline float RandomDouble(std::random_device* rand, double aStart, double aEnd)
	{
		std::default_random_engine engine((*rand)());
		std::uniform_real_distribution<double> distribution(aStart, aEnd);
		return distribution(engine);
	}
}