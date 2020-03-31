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

void ViVertexBatch::Draw(ViTransform aTransform, ViMesh* aMesh, int aMeshSubsection, int64_t aInfo)
{
	ViVertexBatchInstance instance = ViVertexBatchInstance();
	instance.transform = aTransform;
	instance.mesh = aMesh;
	instance.meshSubsection = aMeshSubsection;
	instance.instanced = false;
	instance.instancedCount = 0;
	instance.info = aInfo;

	mInstances.push_back(instance);
}

void ViVertexBatch::DrawString(ViTransform aTransform, ViMaterialFont* aFont, std::string aText)
{
	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	GLuint lastIndex = 0;
	float offX = 0, offY = 0;

	for (char c : aText)
	{
		auto charInfo = aFont->GetCharInfo(c);
		ViMesh* cMesh = charInfo.mesh;

		Draw(aTransform, cMesh);
		aTransform.Translate(vec3(charInfo.offX, charInfo.offY, 0));
		//offX += aFont->GetCharInfo(c).width;

		/*ViGlyphInfo glyph(aFont, c, offX, offY);
		offX = glyph.offX;
		offY = glyph.offY;

		vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[0], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[0]));
		vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[1], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[1]));
		vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[2], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[2]));
		vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[3], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[3]));
		indices.push_back(lastIndex);
		indices.push_back(lastIndex + 1);
		indices.push_back(lastIndex + 2);
		indices.push_back(lastIndex);
		indices.push_back(lastIndex + 2);
		indices.push_back(lastIndex + 3);

		lastIndex += 4;*/
	}

	//TODO cache font character meshes
	/*ViMesh* mesh = new ViMesh(aMaterial, vertices, indices);
	mesh->SetVolatile(true);
	Draw(aTransform, mesh);(*/
}

void ViVertexBatch::Flush()
{
	ViVertexBatchInstance lastInstance;
	bool first = true;
	bool deletedLast = true;	//true by default - in the case that we don't have any instances to flush

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

		ViMeshSubsection subsection = instance.mesh->GetSubsection(instance.meshSubsection);

		bool meshChanged = first || (lastInstance.mesh != instance.mesh);
		bool transformChanged = first || (lastInstance.transform != instance.transform);
		bool materialChanged = first || (lastInstance.mesh->GetSubsection(lastInstance.meshSubsection).material != subsection.material);

		if (!instance.mesh->HasGeneratedGLObjects())
		{
			if (mUsedOtherGLObjects)
			{	//we have to rebind our vao, vbo, and ibo if we we were using a mesh's gl objects
				mUsedOtherGLObjects = false;
				glBindVertexArray(vao);
				glBindBuffer(GL_ARRAY_BUFFER, vbo);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			}

			if (meshChanged)
			{
				GLsizeiptr sizeVert = instance.mesh->GetVerticesSize();
				GLsizeiptr sizeIndex = instance.mesh->GetIndicesSize();

				glBufferData(GL_ARRAY_BUFFER, sizeVert, instance.mesh->GetVertices().data(), GL_STATIC_DRAW);

				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeIndex, instance.mesh->GetIndices().data(), GL_STATIC_DRAW);

				if (mSettings.drawMode == ViVertexBatchSettings::cDRAW_FILLED)
					glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				else if (mSettings.drawMode == ViVertexBatchSettings::cDRAW_LINES)
					glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}

			if (meshChanged || materialChanged)
			{
				instance.mesh->BindSubsection(instance, subsection);
				mSettings.SetTextureSettings();
			}

			if (transformChanged)
				subsection.material->GetProgram()->SetObjectMat(instance.transform.Matrix());

			subsection.material->GetProgram()->SetUniforms(instance);

			if (instance.instanced)
				glDrawElementsInstanced(GL_TRIANGLES, subsection.size, GL_UNSIGNED_INT, (void*)(subsection.start * sizeof(GLuint)), instance.instancedCount);
			else glDrawElements(GL_TRIANGLES, subsection.size, GL_UNSIGNED_INT, (void*)(subsection.start * sizeof(GLuint)));
		}
		else
		{
			mUsedOtherGLObjects = true;

			if (meshChanged)
			{
				//Don't need to worry about unbinding last mesh; after all, we're overwriting those bindings here.
				instance.mesh->Bind();
			}

			if (meshChanged || materialChanged)
			{
				instance.mesh->BindSubsection(instance, subsection);
				mSettings.SetTextureSettings();
			}

			if (transformChanged)
				subsection.material->GetProgram()->SetObjectMat(instance.transform.Matrix());

			subsection.material->GetProgram()->SetUniforms(instance);

			if (instance.instanced)
				glDrawElementsInstanced(GL_TRIANGLES, subsection.size, GL_UNSIGNED_INT, (void*)(subsection.start * sizeof(GLuint)), instance.instancedCount);
			else glDrawElements(GL_TRIANGLES, subsection.size, GL_UNSIGNED_INT, (void*)(subsection.start * sizeof(GLuint)));
		}

		if (!deletedLast && lastInstance.mesh->GetVolatile())
		{//TODO memory leak
			delete lastInstance.mesh;
			deletedLast = true;
		}
		else deletedLast = false;

		lastInstance = instance;
		first = false;
	}

	if (!deletedLast && lastInstance.mesh->GetVolatile())
		delete lastInstance.mesh;

	mInstances.clear();
}

void ViVertexBatch::SetSettings(ViVertexBatchSettings aSettings)
{
	Flush();
	mSettings = aSettings;
}
