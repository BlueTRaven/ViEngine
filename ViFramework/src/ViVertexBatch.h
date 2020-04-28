#pragma once

#include <GL/glew.h>

#include "ViVertexBatchInstance.h"
#include "ViFont.h"
#include "ViProgram.h"
#include "ViVertexBatchSettings.h"
#include "ViUtil.h"
#include "ViMaterialFont.h"

#define VERTEX_BATCH ENVIRONMENT->GetVertexBatch()

class VIENGINE_EXPORT ViVertexBatch
{
public:
	ViVertexBatch();

	void Init(ViVertexBatchSettings aSettings);
	void Draw(ViTransform aTransform, ViMesh* aMesh, ViProgram* aProgram, ViTexture* aTexture, int aTextureBinding, int64_t aInfo = 0);
	void DrawString(ViTransform aTransform, ViMaterialFont* aFont, std::string text);

	void Flush();

	void SetSettings(ViVertexBatchSettings aSettings);

private:
	DISABLE_WARNING_PUSH
	DISABLE_WARNING(4251)
	std::vector<ViVertexBatchInstance> mInstances;
	ViVertexBatchSettings mSettings;
	DISABLE_WARNING_POP

	bool mUsedOtherGLObjects;

	GLuint vao;
	GLuint vbo;
	GLuint ibo;
};