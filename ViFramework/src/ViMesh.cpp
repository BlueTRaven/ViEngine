#include "ViMesh.h"

ViMesh::ViMesh(ViMaterial* aMaterial, std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices) :
	mVertices(aVertices),
	mIndices(aIndices),
	mVerticesSize(aVertices.size() * sizeof(ViVertex)),
	mIndicesSize(aIndices.size() * sizeof(GLuint)),
	mCanGenerateGLObjects(false),
	mHasGLObjects(false),
	mVAO(0),
	mVBO(0),
	mIBO(0),
	mVolatile(false)
{
	mSubsections = std::vector<ViMeshSubsection>{ ViMeshSubsection(aMaterial, 0, mIndicesSize) };
}

ViMesh::ViMesh(std::vector<ViMeshSubsection> aSubsections, std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices) : 
	mSubsections(aSubsections),
	mVertices(aVertices),
	mIndices(aIndices),
	mVerticesSize(aVertices.size() * sizeof(ViVertex)),
	mIndicesSize(aIndices.size() * sizeof(GLuint)),
	mCanGenerateGLObjects(false),
	mHasGLObjects(false),
	mVAO(0),
	mVBO(0),
	mIBO(0),
	mVolatile(false)
{
}

void ViMesh::Merge(ViMesh* aMesh...)
{
	std::vector<ViMesh*> aMeshes = { aMesh };

	for (ViMesh* mesh : aMeshes)
	{
		int oldCount = (int)mVertices.size() - 1;
		mVertices.insert(mVertices.end(), mesh->GetVertices().begin(), mesh->GetVertices().end());
		mVerticesSize += mesh->GetVerticesSize();

		std::vector<GLuint> indices = mesh->GetIndices();
		for (GLuint index : indices)
		{	//indices need to be fixed since they're relative to the old mesh
			mIndices.push_back(index + oldCount);
		}
		mIndicesSize += mesh->GetIndicesSize();
	}
}

void ViMesh::GenerateGLObjects()
{
	mHasGLObjects = true;

	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);
}

bool ViMesh::HasGeneratedGLObjects()
{
	return mHasGLObjects;
}

void ViMesh::Bind()
{
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

}

void ViMesh::BindSubsection(ViMeshSubsection aMeshSubsection)
{
	glUseProgram(aMeshSubsection.material->GetProgram()->GetId());
	aMeshSubsection.material->GetProgram()->BindAttributes();
	aMeshSubsection.material->GetProgram()->SetUniforms();

	if (aMeshSubsection.material->GetTexture() != nullptr)
		glBindTexture(GL_TEXTURE_2D, aMeshSubsection.material->GetTexture()->GetId());
}

ViMeshSubsection ViMesh::GetSubsection(int aIndex)
{
	if (aIndex < 0 || aIndex >= mSubsections.size())
	{
		printf("Error: Could not get subsection %i as it was out of bounds. Size is %i.\n", aIndex, (int)mSubsections.size());
		return ViMeshSubsection();
	}

	return mSubsections[aIndex];
}

void ViMesh::Unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ViMesh::UploadData()
{
	if (!mHasGLObjects)
		GenerateGLObjects();

	Bind();

	glBufferData(GL_ARRAY_BUFFER, mVerticesSize, mVertices.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndicesSize, mIndices.data(), GL_STATIC_DRAW);

	//mMaterial->GetProgram()->BindAttributes(true);

	Unbind();
}

ViMesh* ViMesh::MakeQuad(ViMaterial* aMaterial, vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD)
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

	return new ViMesh(aMaterial, vertices, indices);
}

void ViMesh::MakeQuadRaw(vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD, std::vector<ViVertex> &aVertices, std::vector<GLuint> &aIndices, bool aOverwrite)
{
	if (aOverwrite)
	{
		aVertices.clear();
		aIndices.clear();
	}

	int offset = (int)aVertices.size();
	aIndices.push_back(offset + 0);
	aIndices.push_back(offset + 1);
	aIndices.push_back(offset + 3);
	aIndices.push_back(offset + 1);
	aIndices.push_back(offset + 2);
	aIndices.push_back(offset + 3);

	aVertices.push_back(ViVertex(pointA, vicolors::WHITE, glm::vec2(0.0, 0.0)));
	aVertices.push_back(ViVertex(pointB, vicolors::WHITE, glm::vec2(1.0, 0.0)));
	aVertices.push_back(ViVertex(pointC, vicolors::WHITE, glm::vec2(1.0, 1.0)));
	aVertices.push_back(ViVertex(pointD, vicolors::WHITE, glm::vec2(0.0, 1.0)));
}

ViMesh* ViMesh::MakeUCube(ViMaterial* aMaterial, vec3 min, vec3 max, int aFaces)
{
	if (aFaces == cFACE_NONE)
		return ViMesh::GetEmpty();
	//<near/far - n/f>_<top/bottom - t/b>_<left/right - l/r>

	vec3 l_t_n = { min.x, min.y, min.z };
	vec3 r_t_n = { max.x, min.y, min.z };
	vec3 r_b_n = { max.x, max.y, min.z };
	vec3 l_b_n = { min.x, max.y, min.z };

	vec3 l_t_f = { min.x, min.y, max.z };
	vec3 r_t_f = { max.x, min.y, max.z };
	vec3 r_b_f = { max.x, max.y, max.z };
	vec3 l_b_f = { min.x, max.y, max.z };

	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<ViMeshSubsection> subsections;

	int numIndices = 0;
	if (aFaces & cFACE_FRONT)
	{
		ViMesh::MakeQuadRaw(l_t_n, r_t_n, r_b_n, l_b_n, vertices, indices); //front face
		subsections.push_back(ViMeshSubsection(aMaterial, numIndices * sizeof(GLuint), 6));
		numIndices += 6;
	}
	if (aFaces & cFACE_RIGHT)
	{
		ViMesh::MakeQuadRaw(r_t_n, r_t_f, r_b_f, r_b_n, vertices, indices);	//right face
		subsections.push_back(ViMeshSubsection(aMaterial, numIndices * sizeof(GLuint), 6));
		numIndices += 6;
	}
	if (aFaces & cFACE_BACK)
	{
		ViMesh::MakeQuadRaw(r_t_f, l_t_f, l_b_f, r_b_f, vertices, indices);	//back face
		subsections.push_back(ViMeshSubsection(aMaterial, numIndices * sizeof(GLuint), 6));
		numIndices += 6;
	}
	if (aFaces & cFACE_LEFT)
	{
		ViMesh::MakeQuadRaw(l_t_f, l_t_n, l_b_n, l_b_f, vertices, indices);	//left face
		subsections.push_back(ViMeshSubsection(aMaterial, numIndices * sizeof(GLuint), 6));
		numIndices += 6;
	}
	if (aFaces & cFACE_TOP)
	{
		ViMesh::MakeQuadRaw(l_t_f, r_t_f, r_t_n, l_t_n, vertices, indices);	//top face
		subsections.push_back(ViMeshSubsection(aMaterial, numIndices * sizeof(GLuint), 6));
		numIndices += 6;
	}
	if (aFaces & cFACE_BOTTOM)
	{
		ViMesh::MakeQuadRaw(r_b_f, l_b_f, l_b_n, r_b_n, vertices, indices);	//bottom face
		subsections.push_back(ViMeshSubsection(aMaterial, numIndices * sizeof(GLuint), 6));
		numIndices += 6;
	}

	/*std::vector<ViMeshSubsection> subsections =
	{
		ViMeshSubsection(aMaterial, 0, 6 * sizeof(GLuint)),
		ViMeshSubsection(aMaterial, 6, 6 * sizeof(GLuint)),
		ViMeshSubsection(aMaterial, 12, 6 * sizeof(GLuint)),
		ViMeshSubsection(aMaterial, 18, 6 * sizeof(GLuint)),
		ViMeshSubsection(aMaterial, 24, 6 * sizeof(GLuint)),
		ViMeshSubsection(aMaterial, 30, 6 * sizeof(GLuint))
	};*/

	ViMesh* mesh = new ViMesh(subsections, vertices, indices);

	return mesh;
}

ViMesh* ViMesh::Empty = nullptr;

ViMesh* ViMesh::GetEmpty()
{
	if (Empty == nullptr)
		Empty = new ViMesh(nullptr, {}, {});

	return Empty;
}