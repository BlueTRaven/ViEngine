#pragma once

#include "ViMaterial.h"
#include "ViMesh.h"
#include "ViTransform.h"

struct ViVertexBatchInstance
{
	ViMesh* mesh;
	int meshSubsection;
	ViTransform transform;
};