#pragma once

#include "ViTexture.h"
#include "ViTextureDrawInstance.h"
#include "ViProgram.h"
#include "ViMesh.h"
#include "ViTransform.h"

struct ViVertexBatchInstance
{
	ViMesh* mesh;
	ViTextureDrawInstance* texture;
	ViProgram* program;
	ViTransform transform;

	//extra information, if necessary
	//Store information about the draw call here.
	//It's up to the executing program to determine how to use this.
	int64_t info;
};