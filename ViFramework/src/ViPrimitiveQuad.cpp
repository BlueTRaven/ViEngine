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

void ViPrimitiveQuad::Resize(vec3 aBottomLeft, vec3 aTopRight)
{
	if (mMesh != nullptr)
		delete mMesh;
	//TODO the math might become wrong on this...
	//mMesh = ViMesh::MakeQuad(vec3(aTopRight.x, aBottomLeft.y, aTopRight.z), aBottomLeft, vec3(aBottomLeft.x, aTopRight.y, aBottomLeft.z), aTopRight, vec3(0, 0, -1));

	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	vertices.push_back(ViVertex(vec3(0, 0, -1), aBottomLeft, vicolors::WHITE, vec2(0, 0)));
	vertices.push_back(ViVertex(vec3(0, 0, -1), vec3(aBottomLeft.x, aTopRight.y, aBottomLeft.z), vicolors::WHITE, vec2(0, 1)));
	vertices.push_back(ViVertex(vec3(0, 0, -1), aTopRight, vicolors::WHITE, vec2(1, 1)));
	vertices.push_back(ViVertex(vec3(0, 0, -1), vec3(aTopRight.x, aBottomLeft.y, aTopRight.z), vicolors::WHITE, vec2(1, 0)));

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	mMesh = new ViMesh(vertices, indices);
}
