#pragma once

#include "ViMesh.h"
#include "ViUtil.h"

class ViPrimitiveQuad
{
public:
	ViPrimitiveQuad(vec3 aTopLeft, vec3 aBottomRight);
	~ViPrimitiveQuad();

	void Resize(vec3 aTopLeft, vec3 aBottomRight);

	vi_property_get_named(ViMesh*, mMesh, Mesh);

private:
	ViMesh* mMesh;
};