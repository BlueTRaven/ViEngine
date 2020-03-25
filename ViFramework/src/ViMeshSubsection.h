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

	ViMeshSubsection(ViMaterial* aMaterial, GLuint aStart, GLsizeiptr aSize) :
		material(aMaterial),
		start(aStart),
		size(aSize)
	{ }


	ViMaterial* material;
	GLuint start;
	GLsizeiptr size;
};