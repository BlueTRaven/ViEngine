#include "ViMesh.h"

#include "ViVertexBatchInstance.h"

ViMesh::ViMesh(std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices) :
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

ViMesh::~ViMesh()
{
	if (mHasGLObjects)
	{
		glDeleteVertexArrays(1, &mVAO);
		glDeleteBuffers(1, &mVBO);
		glDeleteBuffers(1, &mIBO);
	}
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

ViMesh* ViMesh::MakeQuad(vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD, vec3 nrm)
{
	std::vector<ViVertex> vertices
	{
		ViVertex(nrm, pointA, vicolors::WHITE, glm::vec2(0.0, 0.0)),
		ViVertex(nrm, pointB, vicolors::WHITE, glm::vec2(1.0, 0.0)),
		ViVertex(nrm, pointC, vicolors::WHITE, glm::vec2(1.0, 1.0)),
		ViVertex(nrm, pointD, vicolors::WHITE, glm::vec2(0.0, 1.0))
	};

	std::vector<GLuint> indices
	{
		0, 1, 3, 1, 2, 3
	};

	return new ViMesh(vertices, indices);
}

void ViMesh::MakeQuadRaw(vec3 pointA, vec3 pointB, vec3 pointC, vec3 pointD, vec3 nrm, std::vector<ViVertex> &aVertices, std::vector<GLuint> &aIndices, ViColorGL aColor, bool aOverwrite)
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

	aVertices.push_back(ViVertex(nrm, pointA, aColor, glm::vec2(0.0, 0.0)));
	aVertices.push_back(ViVertex(nrm, pointB, aColor, glm::vec2(1.0, 0.0)));
	aVertices.push_back(ViVertex(nrm, pointC, aColor, glm::vec2(1.0, 1.0)));
	aVertices.push_back(ViVertex(nrm, pointD, aColor, glm::vec2(0.0, 1.0)));
}

ViMesh* ViMesh::MakeUCube(vec3 min, vec3 max, uint8_t aFaces, ViColorGL aColor)
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

	int numIndices = 0;
	if (aFaces & cFACE_FRONT)
	{
		ViMesh::MakeQuadRaw(l_t_n, r_t_n, r_b_n, l_b_n, { 0.0, 0.0, -1.0 }, vertices, indices, aColor); //front face
		numIndices += 6;
	}
	if (aFaces & cFACE_RIGHT)
	{
		ViMesh::MakeQuadRaw(r_t_n, r_t_f, r_b_f, r_b_n, { 1.0, 0.0, 0.0 }, vertices, indices, aColor);	//right face
		numIndices += 6;
	}
	if (aFaces & cFACE_BACK)
	{
		ViMesh::MakeQuadRaw(r_t_f, l_t_f, l_b_f, r_b_f, { 0.0, 0.0, 1.0 }, vertices, indices, aColor);	//back face
		numIndices += 6;
	}
	if (aFaces & cFACE_LEFT)
	{
		ViMesh::MakeQuadRaw(l_t_f, l_t_n, l_b_n, l_b_f, { -1.0, 0.0, 0.0 }, vertices, indices, aColor);	//left face
		numIndices += 6;
	}
	if (aFaces & cFACE_TOP)
	{
		ViMesh::MakeQuadRaw(l_t_f, r_t_f, r_t_n, l_t_n, { 0.0, -1.0, 0.0 }, vertices, indices, aColor);	//top face
		numIndices += 6;
	}
	if (aFaces & cFACE_BOTTOM)
	{
		ViMesh::MakeQuadRaw(r_b_f, l_b_f, l_b_n, r_b_n, { 0.0, 1.0, 0.0 }, vertices, indices, aColor);	//bottom face
		numIndices += 6;
	}

	ViMesh* mesh = new ViMesh(vertices, indices);

	return mesh;
}

void ViMesh::MakeUCubeRaw(vec3 min, vec3 max, uint8_t aFaces, std::vector<ViVertex>& aVertices, std::vector<GLuint>& aIndices, ViColorGL aColor, bool aOverwrite)
{
	if (aFaces == cFACE_NONE)
		return;
	//<near/far - n/f>_<top/bottom - t/b>_<left/right - l/r>

	vec3 l_t_n = { min.x, min.y, min.z };
	vec3 r_t_n = { max.x, min.y, min.z };
	vec3 r_b_n = { max.x, max.y, min.z };
	vec3 l_b_n = { min.x, max.y, min.z };

	vec3 l_t_f = { min.x, min.y, max.z };
	vec3 r_t_f = { max.x, min.y, max.z };
	vec3 r_b_f = { max.x, max.y, max.z };
	vec3 l_b_f = { min.x, max.y, max.z };

	if (aOverwrite)
	{
		aVertices.clear();
		aIndices.clear();
	}

	int numIndices = 0;
	if (aFaces & cFACE_FRONT)
	{
		ViMesh::MakeQuadRaw(l_t_n, r_t_n, r_b_n, l_b_n, { 0.0, 0.0, -1.0 }, aVertices, aIndices, aColor); //front face
		numIndices += 6;
	}
	if (aFaces & cFACE_RIGHT)
	{
		ViMesh::MakeQuadRaw(r_t_n, r_t_f, r_b_f, r_b_n, { 1.0, 0.0, 0.0 }, aVertices, aIndices, aColor);	//right face
		numIndices += 6;
	}
	if (aFaces & cFACE_BACK)
	{
		ViMesh::MakeQuadRaw(r_t_f, l_t_f, l_b_f, r_b_f, { 0.0, 0.0, 1.0 }, aVertices, aIndices, aColor);	//back face
		numIndices += 6;
	}
	if (aFaces & cFACE_LEFT)
	{
		ViMesh::MakeQuadRaw(l_t_f, l_t_n, l_b_n, l_b_f, { -1.0, 0.0, 0.0 }, aVertices, aIndices, aColor);	//left face
		numIndices += 6;
	}
	if (aFaces & cFACE_TOP)
	{
		ViMesh::MakeQuadRaw(l_t_f, r_t_f, r_t_n, l_t_n, { 0.0, -1.0, 0.0 }, aVertices, aIndices, aColor);	//top face
		numIndices += 6;
	}
	if (aFaces & cFACE_BOTTOM)
	{
		ViMesh::MakeQuadRaw(r_b_f, l_b_f, l_b_n, r_b_n, { 0.0, 1.0, 0.0 }, aVertices, aIndices, aColor);	//bottom face
		numIndices += 6;
	}
}

void ViMesh::MergeInto(std::vector<ViVertex>& aIntoVertices, std::vector<GLuint>& aIntoIndices, ViMesh* aMesh)
{
}

ViMesh* ViMesh::Empty = nullptr;

ViMesh* ViMesh::GetEmpty()
{
	if (Empty == nullptr)
		Empty = new ViMesh({}, {});

	return Empty;
}