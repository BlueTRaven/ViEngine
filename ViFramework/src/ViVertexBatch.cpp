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
	if (aMesh == ViMesh::GetEmpty())
		return;	//don't draw, since Empty has nothing to draw...

	std::vector<ViMeshSubsection> subsections = aMesh->GetSubsections();

	for (int i = 0; i < subsections.size(); i++)
	{
		Draw(aTransform, aMesh, i);
	}
}

void ViVertexBatch::Draw(ViTransform aTransform, ViMesh* aMesh, int aMeshSubsection)
{
	ViVertexBatchInstance instance = ViVertexBatchInstance();
	instance.transform = aTransform;
	instance.mesh = aMesh;
	instance.meshSubsection = aMeshSubsection;

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

	std::sort(mInstances.begin(), mInstances.end(), 
		[](ViVertexBatchInstance a, ViVertexBatchInstance b) 
	{ 
		return a.mesh < b.mesh; 
	});

	for (ViVertexBatchInstance instance : mInstances)
	{
		if (instance.mesh == ViMesh::GetEmpty())
			continue;	//don't draw if we're the empty instance mesh

		GLsizeiptr sizeVert = instance.mesh->GetVerticesSize();
		GLsizeiptr sizeIndex = instance.mesh->GetIndicesSize();

		bool meshChanged = first || (lastInstance.mesh != instance.mesh);
		bool transformChanged = first || (lastInstance.transform != instance.transform);

		if (!instance.mesh->HasGeneratedGLObjects())
		{
			ViMeshSubsection subsection = instance.mesh->GetSubsection(instance.meshSubsection);

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

				instance.mesh->BindSubsection(subsection);
			}

			if (transformChanged)
				subsection.material->GetProgram()->SetObjectMat(instance.transform.Matrix());

			if (instance.mesh->GetVolatile())
				delete instance.mesh;

			void* offset = subsection.start == 0 ? nullptr : (void*)subsection.start;
			glDrawElements(GL_TRIANGLES, (GLsizei)subsection.size, GL_UNSIGNED_INT, offset);
		}
		else
		{
			mUsedOtherGLObjects = true;

			ViMeshSubsection subsection = instance.mesh->GetSubsection(instance.meshSubsection);

			if (meshChanged)
			{
				//Don't need to worry about unbinding last mesh; after all, we're overwriting thsoe bindings here.
				instance.mesh->Bind();
				instance.mesh->BindSubsection(subsection);
				mSettings.SetTextureSettings();
			}

			if (transformChanged)
				subsection.material->GetProgram()->SetObjectMat(instance.transform.Matrix());

			if (instance.mesh->GetVolatile())
				delete instance.mesh;

			void* offset = subsection.start == 0 ? nullptr : (void*)subsection.start;
			glDrawElements(GL_TRIANGLES, (GLsizei)subsection.size, GL_UNSIGNED_INT, offset);
		}

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
