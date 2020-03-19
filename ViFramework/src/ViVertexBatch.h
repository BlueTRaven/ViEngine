#pragma once

#include <gl/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <vector>

#include "ViColorsGL.h"
#include "ViVertex.h"
#include "ViMaterial.h"
#include "ViFont.h"
#include "ViGlyphInfo.h"
#include "ViTransform.h"
#include "ViVertexBatchSettings.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViVertexBatch
{
public:
	ViVertexBatch();

	void Init(ViVertexBatchSettings aSettings);

	//Adds all vertices to be drawn next time Flush is called.
	//If the material passed in is not the same as the current material (mMaterial), automatically flushes.
	void Draw(ViMaterial* aMaterial, ViTransform aTransform, std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices);
	void DrawQuad(ViMaterial* aMaterial, ViTransform aTransform, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD);
	void DrawString(ViMaterial* aMaterial, ViTransform aTransform, ViFont* aFont, std::string text);

	//Takes the current vertices and indices and draws them.
	//Clears both vectors afterwards.
	void Flush();

	void SetSettings(ViVertexBatchSettings aSettings);

	vi_property_get_named(ViVertexBatchSettings, mSettings, Settings)

private:
	bool ShouldFlush(ViMaterial* aMaterial, ViTransform aTransform);

	GLuint vao = 0;
	GLuint vbo = 0;
	GLuint ibo = 0;

	ViVertexBatchSettings mSettings;

	ViMaterial* mMaterial;
	ViTransform mTransform;

	DISABLE_WARNING_PUSH
		DISABLE_WARNING(4251)
		std::vector<GLuint> indices;
	std::vector<ViVertex> vertices;
	DISABLE_WARNING_POP

		GLuint BindAndGetVertexAttribute(ViVertexAttribute* attribute);

	bool mHasAnything = false;
};
