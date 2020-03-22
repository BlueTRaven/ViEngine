#pragma once

#include <GL/glew.h>

#include "ViMesh.h"
#include "ViTransform.h"
#include "ViMaterial.h"
#include "ViProgram.h"
#include "ViVertexBatchSettings.h"
#include "ViUtil.h"

class ViVertexBatch_
{
public:
	void Init(ViVertexBatchSettings aSettings);
	void Draw(ViMaterial* aMaterial, ViTransform aTransform, ViMesh* aMesh);
	
	void Flush();

	void SetSettings(ViVertexBatchSettings aSettings);

private:
	struct ViVertexBatchInstance 
	{
		ViMesh* mesh;
		ViTransform transform;
		ViMaterial* material;
	};

	std::vector<ViVertexBatchInstance> mInstances;
	ViVertexBatchSettings mSettings;

	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};