#include "ViVertexBatch.h"

ViVertexBatch::ViVertexBatch() :
	mSettings(ViVertexBatchSettings::Default)
{ }

void ViVertexBatch::Init(ViVertexBatchSettings aSettings)
{
	mSettings = aSettings;

	//We only need one of each - so we can gen and bind them here
	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

	glGenBuffers(1, &ibo);
}

void ViVertexBatch::Draw(ViMaterial* aMaterial, ViTransform aTransform, std::vector<ViVertex> vertices, std::vector<GLuint> indices)
{
	if (ShouldFlush(aMaterial, aTransform))
		Flush();

	mHasAnything = true;
	mMaterial = aMaterial;
	mTransform = aTransform;

	assert(indices.size() % 3 == 0);

	this->vertices.insert(this->vertices.end(), vertices.begin(), vertices.end());
	this->indices.insert(this->indices.end(), indices.begin(), indices.end());
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

void ViVertexBatch::Flush()
{
	if (!mHasAnything)
		return;

	if (mSettings.cullMode != ViVertexBatchSettings::cCULL_NONE)
	{
		glEnable(GL_CULL_FACE);
		glFrontFace(mSettings.cullMode == ViVertexBatchSettings::cCULL_CW ? GL_CW : GL_CCW);
		glCullFace(GL_BACK);
	}
	else glDisable(GL_CULL_FACE);

	if (mSettings.depthMode != ViVertexBatchSettings::cDEPTH_NONE)
	{
		glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(mSettings.depthMode == ViVertexBatchSettings::cDEPTH_LESS ? GL_LESS : GL_GREATER);
	}
	else glDisable(GL_DEPTH_TEST);

	glBindVertexArray(vao);		//binds the vertex array to vao ptr
	glBindBuffer(GL_ARRAY_BUFFER, vbo);	//binds the GL_ARRAY_BUFFER to vbo ptr
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);	//binds the GL_ELEMENT_ARRAY_BUFFER to ind ptr

	GLsizeiptr sizeVert = vertices.size() * sizeof(ViVertex);
	GLsizeiptr sizeIndex = indices.size() * sizeof(GLuint);

	glBufferData(GL_ARRAY_BUFFER, sizeVert, vertices.data(), GL_STATIC_DRAW);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndex, indices.data(), GL_STATIC_DRAW);

	glUseProgram(mMaterial->GetProgram()->Get_program());

	mMaterial->GetProgram()->SetUniforms();
	mMaterial->GetProgram()->SetObjectMat(mTransform.Matrix());

	std::vector<GLuint> boundVertexAttributes;	//these need to be unbound after drawing, so we need to keep track of them.
	for (ViVertexAttribute* attribute : mMaterial->GetProgram()->Get_mVertexAttributes())
	{
		GLuint attrib = BindAndGetVertexAttribute(attribute);

		if (attrib >= 0)
			boundVertexAttributes.push_back(attrib);
	}

	//note that we don't have to have a texture, so we simply skip this step if we don't have one
	if (mMaterial->GetTexture() != nullptr)
	{
		GLuint id = mMaterial->GetTexture()->GetId();
		glBindTexture(GL_TEXTURE_2D, id);

		switch (mSettings.textureMode)
		{
		case ViVertexBatchSettings::TexMode::cWRAP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case ViVertexBatchSettings::TexMode::cWRAPM_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case ViVertexBatchSettings::TexMode::cCLAMP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case ViVertexBatchSettings::TexMode::cWRAP_POINT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case ViVertexBatchSettings::TexMode::cWRAPM_POINT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case ViVertexBatchSettings::TexMode::cCLAMP_POINT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
	}

	//Note that we pass NULL into offsets
	//We're only going to be drawing once per material; which means all indices will belong to that material
	//therefore we don't need any offsets
	glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, NULL);

	for (auto attribute : boundVertexAttributes)
		glDisableVertexAttribArray(attribute);

	vertices.clear();
	indices.clear();

	mHasAnything = false;

	mMaterial->GetProgram()->SetDirty(false);
}

void ViVertexBatch::SetSettings(ViVertexBatchSettings aSettings)
{
	Flush();
	mSettings = aSettings;
}

GLuint ViVertexBatch::BindAndGetVertexAttribute(ViVertexAttribute* attribute)
{
	if (attribute->Get_id() >= 0)
	{
		GLuint id = attribute->Get_id();
		glEnableVertexAttribArray(id);
		glVertexAttribPointer(id, attribute->Get_elements(), GL_FLOAT, GL_FALSE, (GLsizei)attribute->Get_size(), attribute->Get_offset());
		glBindAttribLocation(mMaterial->GetProgram()->Get_program(), id, attribute->Get_name().c_str());

		return id;
	}
	else 
	{
		printf("Error: attribute not yet bound. %s\n", attribute->Get_name().c_str());
	}

	return -1;
}

bool ViVertexBatch::ShouldFlush(ViMaterial* aMaterial, ViTransform aTransform)
{
	return aMaterial != mMaterial || aTransform != mTransform || aMaterial->GetProgram()->GetDirty();
}
