#pragma once

#include <vector>

#include "ViMaterial.h"
#include "ViVertex.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViMesh
{
public:
	ViMesh(ViMaterial* aMaterial, std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices);

	vi_property_named(ViMaterial*, mMaterial, Material);
	vi_property_named(std::vector<ViVertex>, mVertices, Vertices);
	vi_property_named(std::vector<GLuint>, mIndices, Indices);

	//If the material is the same, combines, then destroys, aMesh. If the material is not the same, the mesh cannot be combined; does nothing.
	void Combine(ViMesh* aMesh);

	//Makes a quad ViMesh.
	static ViMesh* MakeQuad(ViMaterial* aMaterial, vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD);
	//Makes a quad. If aOverwrite is false, adds to aVertices and aIndices; otherwise, overwrites them completely.
	static void MakeQuadRaw(vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD, std::vector<ViVertex> &aVertices, std::vector<GLuint> &aIndices, bool aOverwrite = false);
	//Makes an unrotated cube. This assumes it is axis aligned in world space, and thus takes only two points, a minimum and maximum. min = x left, y top, z near. max = x right, y bottom, z far
	static ViMesh* MakeUCube(ViMaterial* aMaterial, vec3 min, vec3 max);
};