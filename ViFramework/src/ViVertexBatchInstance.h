#pragma once

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViTransform.h"

struct ViVertexBatchInstance
{
	ViMesh* mesh;
	int meshSubsection;
	ViTransform transform;

	//extra information, if necessary
	//Store information about the draw call here.
	//It's up to the executing program to determine how to use this.
	int64_t info;	

	bool instanced;	//whether this draw call should be an instanced draw call or not
	int instancedCount;
};