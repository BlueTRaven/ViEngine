#pragma once

#include "glm/glm.hpp"
#include "ViUtil.h"
#include "ViMesh.h"
#include "ViColorsGL.h"

namespace vigame
{
	class Cube
	{
	public:
		Cube(ViMaterial* aMaterial);

		vi_property_named(ViMesh*, mMesh, Mesh);

	private:
		void CreateMesh(ViMaterial* aMaterial);
	};
}