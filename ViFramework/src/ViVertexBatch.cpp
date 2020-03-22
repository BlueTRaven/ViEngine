#include "ViVertexBatch.h"

ViVertexBatch::ViVertexBatch() :
	mSettings(ViVertexBatchSettings::Default),
	mMaterial(nullptr)
{ }

void ViVertexBatch::Init(ViVertexBatchSettings aSettings)
{
	mSettings = aSettings;

	//We only need one of each - so we can gen and bind them here
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void ViVertexBatch::Draw(ViMaterial* aMaterial, ViTransform aTransform, const std::vector<ViVertex>& aVertices, const std::vector<GLuint>& aIndices)
{
	FlushMode mode = ShouldFlush(aMaterial, aTransform);
	Flush(mode);

	mMaterial = aMaterial;
	if (mode & cFLUSH_PROGRAM)
		mMaterialChanged = true;
	mTransform = aTransform;
	if (mode & cFLUSH_TRANSFORM)
		mTransformChanged = true;

	assert(aIndices.size() % 3 == 0);

	if (mHasAnything)
	{
		mVertices = aVertices;
		mIndices = aIndices;

		mHasAnything = true;
	}
	else 
	{
		if (aVertices != mVertices)
			mVertices.insert(mVertices.end(), aVertices.begin(), aVertices.end());
		if (aIndices != mIndices)
			mIndices.insert(mIndices.end(), aIndices.begin(), aIndices.end());
	}
}

void ViVertexBatch::DrawMesh(ViMesh* aMesh, ViTransform aTransform)
{
	Draw(aMesh->GetMaterial(), aTransform, aMesh->GetVertices(), aMesh->GetIndices());
}

void ViVertexBatch::DrawQuad(ViMaterial * aMaterial, ViTransform aTransform, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC, glm::vec3 pointD)
{
	std::vector<ViVertex> vertices
	{
		ViVertex(pointA, vicolors::WHITE, glm::vec2(0.0, 0.0)),
		ViVertex(pointB, vicolors::WHITE, glm::vec2(1.0, 0.0)),
		ViVertex(pointC, vicolors::WHITE, glm::vec2(1.0, 1.0)),
		ViVertex(pointD, vicolors::WHITE, glm::vec2(0.0, 1.0))
	};

	std::vector<GLuint> indices
	{
		0, 1, 3, 1, 2, 3
	};

	Draw(aMaterial, aTransform, vertices, indices);
}

void ViVertexBatch::DrawString(ViMaterial* aMaterial, ViTransform aTransform, ViFont* aFont, std::string aText)
{
	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	GLuint lastIndex = 0;
	float offX = 0, offY = 0;

	for (char c : aText)
	{
		ViGlyphInfo glyph(aFont, c, offX, offY);
		offX = glyph.offX;
		offY = glyph.offY;

		vertices.emplace_back(ViVertex(glyph.positions[0], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[0]));
		vertices.emplace_back(ViVertex(glyph.positions[1], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[1]));
		vertices.emplace_back(ViVertex(glyph.positions[2], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[2]));
		vertices.emplace_back(ViVertex(glyph.positions[3], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[3]));
		indices.push_back(lastIndex);
		indices.push_back(lastIndex + 1);
		indices.push_back(lastIndex + 2);
		indices.push_back(lastIndex);
		indices.push_back(lastIndex + 2);
		indices.push_back(lastIndex + 3);

		lastIndex += 4;
	}

	Draw(aMaterial, aTransform, vertices, indices);
}

void ViVertexBatch::Flush(int aFlushMode)
{
	if (!mHasAnything || aFlushMode == FlushMode::cFLUSH_NONE)
		return;

	if (mSettingsChanged)
	{
		mSettings.SetSettings();
	}

	if (mVerticesChanged)
	{
		GLsizeiptr sizeVert = mVertices.size() * sizeof(ViVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeVert, mVertices.data(), GL_STATIC_DRAW);
	}

	if (mIndicesChanged)
	{
		GLsizeiptr sizeIndex = mIndices.size() * sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndex, mIndices.data(), GL_STATIC_DRAW);
	}

	if (mMaterialChanged)
	{
		glUseProgram(mMaterial->GetProgram()->GetId());

		mMaterial->GetProgram()->BindAttributes();
		mMaterial->GetProgram()->SetUniforms();
	}

	if (mMaterialChanged || mTransformChanged)
		mMaterial->GetProgram()->SetObjectMat(mTransform.Matrix());
	
	//note that we don't have to have a texture, so we simply skip this step if we don't have one
	if (mMaterial->GetTexture() != nullptr)
	{
		GLuint id = mMaterial->GetTexture()->GetId();
		glBindTexture(GL_TEXTURE_2D, id);
	}

	//Note that we pass NULL into offsets
	//We're only going to be drawing once per material; which means all indices will belong to that material
	//therefore we don't need any offsets
	glDrawElements(GL_TRIANGLES, (GLsizei)mIndices.size(), GL_UNSIGNED_INT, NULL);

	mVertices.clear();
	mIndices.clear();

	mHasAnything = false;

	mMaterial->GetProgram()->SetDirty(false);
	mMaterialChanged = false;
	mTransformChanged = false;
	mSettingsChanged = false;
	mIndicesChanged = false;
	mVerticesChanged = false;
}

void ViVertexBatch::SetSettings(ViVertexBatchSettings aSettings)
{
	Flush(FlushMode::cFLUSH_SETTINGS | FlushMode::cFLUSH_VERTICES);
	mSettingsChanged = true;
	mSettings = aSettings;
}

ViVertexBatch::FlushMode ViVertexBatch::ShouldFlush(ViMaterial* aMaterial, ViTransform aTransform)
{
	if (aMaterial != mMaterial)
		return (FlushMode)(FlushMode::cFLUSH_PROGRAM | FlushMode::cFLUSH_VERTICES);

	FlushMode mode = FlushMode::cFLUSH_VERTICES;

	if (aMaterial->GetProgram()->GetDirty())
		mode = (FlushMode)(mode | FlushMode::cFLUSH_PROGRAM);

	if (aTransform != mTransform)
		mode = (FlushMode)(mode | FlushMode::cFLUSH_TRANSFORM);

	return mode;
}
