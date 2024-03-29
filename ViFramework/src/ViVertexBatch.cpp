#include "ViVertexBatch.h"
#include "ViGlyphInfo.h"
#include "ViEnvironment.h"

ViVertexBatch::ViVertexBatch() :
	mSettings(ViVertexBatchSettings::Default),
	mPrimitiveQuad(new ViPrimitiveQuad(vec3(0), vec3(1)))
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

void ViVertexBatch::Draw(ViTransform aTransform, ViMesh* aMesh, ViProgram* aProgram, ViTexture* aTexture, int aTextureBinding, int64_t aInfo)
{
	if (aMesh == nullptr)
	{
		printf("Error: Null mesh passed into Draw.\n");
		return;
	}

	if (aProgram == nullptr)
	{
		printf("Error: Null program passed into Draw.\n");
		return;
	}

	if (aTexture == nullptr)
	{
		printf("Error: Null texture passed into Draw.\n");
		return;
	}

	ViVertexBatchInstance instance = ViVertexBatchInstance();
	instance.transform = aTransform;
	instance.mesh = aMesh;
	instance.program = aProgram;
	instance.texture = new ViTextureDrawInstance(aTexture, aTextureBinding);
	instance.info = aInfo;

	mInstances.push_back(instance);
}

void ViVertexBatch::Draw(ViTransform aTransform, ViMesh * aMesh, ViProgram * aProgram, std::vector<ViTextureDrawInstance*> aDrawInstances, int64_t aInfo)
{
	if (aMesh == nullptr)
	{
		printf("Error: Null mesh passed into Draw.\n");
		return;
	}

	if (aProgram == nullptr)
	{
		printf("Error: Null program passed into Draw.\n");
		return;
	}

	vi_assert(aDrawInstances.size() > 0, "Error: No textures passed into draw.");

	ViVertexBatchInstance instance = ViVertexBatchInstance();
	instance.transform = aTransform;
	instance.mesh = aMesh;
	instance.program = aProgram;
	instance.texture = nullptr;
	instance.textures = aDrawInstances;
	instance.info = aInfo;

	mInstances.push_back(instance);
}

void ViVertexBatch::DrawQuad(ViTransform aTransform, vec3 aTopLeft, vec3 aBottomRight, ViProgram* aProgram, ViTexture* aTexture, int aTextureBinding)
{
	mPrimitiveQuad->Resize(aTopLeft, aBottomRight);
	Draw(aTransform, mPrimitiveQuad->GetMesh(), aProgram, aTexture, aTextureBinding);
}

void ViVertexBatch::DrawString(ViTransform aTransform, ViMaterialFont* aFont, std::string aText)
{
	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	GLuint lastIndex = 0;
	float offX = 0, offY = 0;

	for (char c : aText)
	{
		auto charInfo = aFont->GetCharMeshInfo(c);
		ViMesh* cMesh = charInfo.mesh;

		//TODO remove all material stuff
		Draw(aTransform, cMesh, aFont->GetProgram(), aFont->GetFont()->GetTexture(), 0);
		aTransform.Translate(vec3(charInfo.offX, charInfo.offY, 0));
	}
}

void ViVertexBatch::Flush()
{
	ViVertexBatchInstance lastInstance;
	bool first = true;
	bool deletedLast = true;	//true by default - in the case that we don't have any instances to flush

	mSettings.SetSettings();

	/*std::sort(mInstances.begin(), mInstances.end(), 
		[](ViVertexBatchInstance a, ViVertexBatchInstance b) 
	{ 
		return a.mesh < b.mesh; 
	});*/

	for (ViVertexBatchInstance instance : mInstances)
	{
		if (instance.mesh == ViMesh::GetEmpty())
			continue;	//don't draw if we're the empty instance mesh

		bool meshChanged = first || (lastInstance.mesh != instance.mesh);
		bool transformChanged = first || (lastInstance.transform != instance.transform);
		bool programChanged = first || (lastInstance.program != instance.program);
		bool textureChanged = first || (lastInstance.texture != instance.texture);

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

			if (meshChanged || textureChanged || programChanged)
			{
				instance.program->Bind(instance);
				if (instance.texture == nullptr && instance.textures.size() > 0)
				{
					//use textures
					for (int i = 0; i < instance.textures.size(); i++)
						instance.textures[i]->Bind();
				}
				else if (instance.textures.size() <= 0 && instance.texture != nullptr)
				{
					//use texture
					instance.texture->Bind();
				}
				mSettings.SetTextureSettings();
			}

			if (transformChanged)
				instance.program->SetObjectMat(instance.transform.Matrix());

			instance.program->SetUniforms(instance);

			glDrawElements(GL_TRIANGLES, instance.mesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
		}
		else
		{
			mUsedOtherGLObjects = true;

			if (meshChanged)
			{
				//Don't need to worry about unbinding last mesh; after all, we're overwriting those bindings here.
				instance.mesh->Bind();
			}

			if (meshChanged || textureChanged || programChanged)
			{
				instance.program->Bind(instance);

				if (instance.texture == nullptr && instance.textures.size() > 0)
				{
					//use textures
					for (int i = 0; i < instance.textures.size(); i++)
						instance.textures[i]->Bind();
				}
				else if (instance.textures.size() <= 0 && instance.texture != nullptr)
				{
					//use texture
					instance.texture->Bind();
				}

				mSettings.SetTextureSettings();
			}

			if (transformChanged)
				instance.program->SetObjectMat(instance.transform.Matrix());

			instance.program->SetUniforms(instance);
			
			glDrawElements(GL_TRIANGLES, instance.mesh->GetIndices().size(), GL_UNSIGNED_INT, nullptr);
		}

		if (!deletedLast && lastInstance.mesh->GetVolatile())
		{//TODO memory leak
			delete lastInstance.mesh;
			deletedLast = true;
		}
		else deletedLast = false;

		lastInstance = instance;
		first = false;

		if (instance.texture != nullptr)
			delete instance.texture;

		if (instance.textures.size() > 0)
			instance.textures.clear();	//clear SHOULD destruct the memory
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

void ViVertexBatch::Clear(ViColorGL aColor, bool aColorBit, bool aDepthBit)
{
	//TODO glClearDepth?
	glClearColor(aColor.r, aColor.g, aColor.b, aColor.a);
	Clear(aColorBit, aDepthBit);
}

void ViVertexBatch::Clear(bool aColorBit, bool aDepthBit)
{
	glClear((aColorBit ? GL_COLOR_BUFFER_BIT : 0) | (aDepthBit ? GL_DEPTH_BUFFER_BIT : 0));
}

void ViVertexBatch::SetTarget(ViFrameBuffer* aFrameBuffer)
{
	Flush();

	if (aFrameBuffer == nullptr)
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else glBindFramebuffer(GL_FRAMEBUFFER, aFrameBuffer->GetId());

	glViewport(0, 0, ENVIRONMENT->GetScreenWidth(), ENVIRONMENT->GetScreenHeight());
}
