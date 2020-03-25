#pragma once

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViTransform.h"

struct ViVertexBatchInstance
{
	ViMesh* mesh;
	ViTransform transform;
};