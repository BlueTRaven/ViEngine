#pragma once

#include <GL/glew.h>

#include "ViVertexBatchInstance.h"
#include "ViMaterial.h"
#include "ViFont.h"
#include "ViProgram.h"
#include "ViVertexBatchSettings.h"
#include "ViUtil.h"

#define VERTEX_BATCH viEnv->GetVertexBatch()

class VIENGINE_EXPORT ViVertexBatch
{
public:
	ViVertexBatch();

	void Init(ViVertexBatchSettings aSettings);
	void Draw(ViTransform aTransform, ViMesh* aMesh);
	void DrawString(ViTransform aTransform, ViMaterial* aMaterial, ViFont* aFont, std::string text);

	void Flush();

	void SetSettings(ViVertexBatchSettings aSettings);

private:
	DISABLE_WARNING_PUSH
	DISABLE_WARNING(4251)
	std::vector<ViVertexBatchInstance> mInstances;
	ViVertexBatchSettings mSettings;
	DISABLE_WARNING_POP

	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};