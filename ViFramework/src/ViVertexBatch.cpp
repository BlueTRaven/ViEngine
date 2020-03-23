#include "ViVertexBatch.h"

#include "ViGlyphInfo.h"

ViVertexBatch::ViVertexBatch() :
	mSettings(ViVertexBatchSettings::Default)
{
}

void ViVertexBatch::Init(ViVertexBatchSettings aSettings)
{
	mSettings = aSettings;

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void ViVertexBatch::Draw(ViTransform aTransform, ViMesh* aMesh)
{
	ViVertexBatchInstance instance = ViVertexBatchInstance();
	instance.transform = aTransform;
	instance.mesh = aMesh;

	mInstances.push_back(instance);
}

void ViVertexBatch::DrawString(ViTransform aTransform, ViMaterial* aMaterial, ViFont* aFont, std::string aText)
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

	//TODO cache font character meshes
	ViMesh* mesh = new ViMesh(aMaterial, vertices, indices);
	mesh->SetVolatile(true);
	Draw(aTransform, mesh);
}

void ViVertexBatch::Flush()
{
	ViVertexBatchInstance lastInstance;
	bool first = true;

	mSettings.SetSettings();

	for (ViVertexBatchInstance instance : mInstances)
	{
		GLsizeiptr sizeVert = instance.mesh->GetVerticesSize();
		GLsizeiptr sizeIndex = instance.mesh->GetIndicesSize();

		bool meshChanged = first || (lastInstance.mesh != instance.mesh);
		bool transformChanged = first || (lastInstance.transform != instance.transform);

		if (!instance.mesh->HasGeneratedGLObjects())
		{
			if (mUsedOtherGLObjects)
			{
				mUsedOtherGLObjects = false;
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			}

			if (meshChanged)
			{
				glBufferData(GL_ARRAY_BUFFER, sizeVert, instance.mesh->GetVertices().data(), GL_STATIC_DRAW);

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndex, instance.mesh->GetIndices().data(), GL_STATIC_DRAW);

				glUseProgram(instance.mesh->GetMaterial()->GetProgram()->GetId());

				instance.mesh->GetMaterial()->GetProgram()->BindAttributes();
				instance.mesh->GetMaterial()->GetProgram()->SetUniforms();

				if (instance.mesh->GetMaterial()->GetTexture() != nullptr)
				{
					glBindTexture(GL_TEXTURE_2D, instance.mesh->GetMaterial()->GetTexture()->GetId());
					mSettings.SetTextureSettings();
				}
			}

			if (transformChanged)
			{
				instance.mesh->GetMaterial()->GetProgram()->SetObjectMat(instance.transform.Matrix());
			}
		}
		else
		{
			mUsedOtherGLObjects = true;
			//Don't need to worry about unbinding last mesh; after all, we're overwriting thsoe bindings here.
			if (meshChanged)
			{
				instance.mesh->Bind();
				instance.mesh->GetMaterial()->GetProgram()->SetUniforms();

				if (instance.mesh->GetMaterial()->GetTexture() != nullptr)
				{
					glBindTexture(GL_TEXTURE_2D, instance.mesh->GetMaterial()->GetTexture()->GetId());
					mSettings.SetTextureSettings();
				}
			}

			if (transformChanged)
				instance.mesh->GetMaterial()->GetProgram()->SetObjectMat(instance.transform.Matrix());
		}

		if (instance.mesh->GetVolatile())
			delete instance.mesh;

		glDrawElements(GL_TRIANGLES, (GLsizei)sizeIndex, GL_UNSIGNED_INT, NULL);

		lastInstance = instance;
		first = false;
	}

	mInstances.clear();
}

void ViVertexBatch::SetSettings(ViVertexBatchSettings aSettings)
{
	Flush();
	mSettings = aSettings;
}
