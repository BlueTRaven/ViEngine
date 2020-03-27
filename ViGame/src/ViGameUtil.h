#pragma once

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
}