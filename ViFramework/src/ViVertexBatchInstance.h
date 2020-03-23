#pragma once

#include "ViMesh.h"
#include "ViTransform.h"

struct ViVertexBatchInstance
{
	ViMesh* mesh;
	ViTransform transform;
};