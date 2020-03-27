#pragma once

#include <GL/glew.h>
#include <vector>

#include "ViMaterial.h"
#include "ViVertex.h"
#include "ViUtil.h"

#include "ViMeshSubsection.h"

class VIENGINE_EXPORT ViMesh
{
public:

	enum CubeFaces
	{
		cFACE_NONE		= 0,
		cFACE_TOP		= 1 << 0,
		cFACE_BOTTOM	= 1 << 1,
		cFACE_LEFT		= 1 << 2,
		cFACE_RIGHT		= 1 << 3,
		cFACE_FRONT		= 1 << 4,
		cFACE_BACK		= 1 << 5,
		cFACE_ALL		= cFACE_TOP | cFACE_BOTTOM | cFACE_LEFT | cFACE_RIGHT | cFACE_FRONT | cFACE_BACK
	};

	ViMesh(ViMaterial* aMaterial, std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices);
	ViMesh(std::vector<ViMeshSubsection> aSubsections, std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices);

	void Merge(ViMesh* aMesh...);
	
	vi_property_named(std::vector<ViMeshSubsection>, mSubsections, Subsections);
	vi_property_named(std::vector<ViVertex>, mVertices, Vertices);
	vi_property_named(std::vector<GLuint>, mIndices, Indices);

	//if a mesh is marked volatile, it will be deleted after being used to draw.
	vi_property_named(bool, mVolatile, Volatile);

	vi_property_get_named(GLsizeiptr, mVerticesSize, VerticesSize);
	vi_property_get_named(GLsizeiptr, mIndicesSize, IndicesSize);

	vi_property_named(bool, mCanGenerateGLObjects, CanGenerateGLObjects);

	void GenerateGLObjects();
	bool HasGeneratedGLObjects();

	void Bind();
	void BindSubsection(ViMeshSubsection aMeshSubsection);

	ViMeshSubsection GetSubsection(int aIndex);

	void Unbind();

	void UploadData();

	//Makes a quad ViMesh.
	static ViMesh* MakeQuad(ViMaterial* aMaterial, vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD, vec3 nrm);
	//Makes a quad. If aOverwrite is false, adds to aVertices and aIndices; otherwise, overwrites them completely.
	static void MakeQuadRaw(vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD, vec3 nrm, std::vector<ViVertex> &aVertices, std::vector<GLuint> &aIndices, ViColorGL aColor, bool aOverwrite = false);
	//Makes an unrotated cube. This assumes it is axis aligned in world space, and thus takes only two points, a minimum and maximum. min = x left, y top, z near. max = x right, y bottom, z far
	static ViMesh* MakeUCube(ViMaterial* aMaterial, vec3 min, vec3 max, int aFaces, ViColorGL aColor);

	static ViMesh* GetEmpty();

private:
	GLsizeiptr mVerticesSize;
	GLsizeiptr mIndicesSize;

	bool mHasGLObjects;
	GLuint mVAO;
	GLuint mVBO;
	GLuint mIBO;

	static ViMesh* Empty;
};