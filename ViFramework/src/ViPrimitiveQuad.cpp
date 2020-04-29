#include "ViPrimitiveQuad.h"

ViPrimitiveQuad::ViPrimitiveQuad(vec3 aTopLeft, vec3 aBottomRight) :
	mMesh(nullptr)
{
	Resize(aTopLeft, aBottomRight);
}

ViPrimitiveQuad::~ViPrimitiveQuad()
{
	delete mMesh;
}

void ViPrimitiveQuad::Resize(vec3 aTopLeft, vec3 aBottomRight)
{
	if (mMesh != nullptr)
		delete mMesh;
	//TODO the math might become wrong on this...
	mMesh = ViMesh::MakeQuad(aTopLeft, vec3(aBottomRight.x, aTopLeft.y, aTopLeft.z), aBottomRight, vec3(aTopLeft.x, aBottomRight.y, aBottomRight.z), vec3(0, 0, -1));
}
