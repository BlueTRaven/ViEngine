#include "ViVertexBatch_.h"

void ViVertexBatch_::Init(ViVertexBatchSettings aSettings)
{
	mSettings = aSettings;

	glGenVertexArrays(1, &vao);

	glGenBuffers(1, &vbo);

	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
}

void ViVertexBatch_::Draw(ViMaterial* aMaterial, ViTransform aTransform, ViMesh* aMesh)
{
	ViVertexBatchInstance instance = ViVertexBatchInstance();
	instance.material = aMaterial;
	instance.transform = aTransform;
	instance.mesh = aMesh;

	mInstances.push_back(instance);
}

void ViVertexBatch_::Flush()
{
	ViVertexBatchInstance lastInstance;
	bool first = true;

	for (ViVertexBatchInstance instance : mInstances)
	{
		GLsizeiptr sizeVert = instance.mesh->GetVertices().size() * sizeof(ViVertex);
		glBufferData(GL_ARRAY_BUFFER, sizeVert, instance.mesh->GetVertices().data(), GL_STATIC_DRAW);

		GLsizeiptr sizeIndex = instance.mesh->GetIndices().size() * sizeof(GLuint);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndex, instance.mesh->GetIndices().data(), GL_STATIC_DRAW);

		glUseProgram(instance.material->GetProgram()->GetId());

		instance.material->GetProgram()->BindAttributes();
		instance.material->GetProgram()->SetUniforms();

		instance.material->GetProgram()->SetObjectMat(instance.transform.Matrix());

		if (instance.material->GetTexture() != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D, instance.material->GetTexture()->GetId());
			mSettings.SetTextureSettings();
		}

		glDrawElements(GL_TRIANGLES, (GLsizei)sizeIndex, GL_UNSIGNED_INT, NULL);

		lastInstance = instance;
		first = false;
	}

	mInstances.clear();
}

void ViVertexBatch_::SetSettings(ViVertexBatchSettings aSettings)
{
	Flush();
	mSettings = aSettings;
}
