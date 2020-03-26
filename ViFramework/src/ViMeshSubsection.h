#pragma once

#include <GL/glew.h>

#include "ViMaterial.h"

struct ViMeshSubsection
{
	ViMeshSubsection() :
		material(nullptr),
		start(0),
		size(0)
	{ }

	ViMeshSubsection(ViMaterial* aMaterial, GLsizeiptr aStart, GLsizei aSize) :
		material(aMaterial),
		start(aStart),
		size(aSize)
	{ }


	ViMaterial* material;
	//The start offset of the subsection, in indices. Note that glDrawElements's 4th parameter requires these in byte offset, rather than index offset; this number is multiplied by sizeof(GLuint) to get the true size.
	GLsizeiptr start;
	//The size of the subsection, in indices.
	GLsizei size;
};