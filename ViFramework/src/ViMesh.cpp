#include "ViMesh.h"

ViMesh::ViMesh(ViMaterial* aMaterial, std::vector<ViVertex> aVertices, std::vector<GLuint> aIndices) :
	mMaterial(aMaterial),
	mVertices(aVertices),
	mIndices(aIndices)
{
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

ViMesh* ViMesh::MakeUCube(ViMaterial* aMaterial, vec3 min, vec3 max)
{
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
	ViMesh::MakeQuadRaw(l_t_n, r_t_n, r_b_n, l_b_n, vertices, indices); //front face
	ViMesh::MakeQuadRaw(r_t_n, r_t_f, r_b_f, r_b_n, vertices, indices);	//right face
	ViMesh::MakeQuadRaw(r_t_f, l_t_f, l_b_f, r_b_f, vertices, indices);	//back face
	ViMesh::MakeQuadRaw(l_t_f, l_t_n, l_b_n, l_b_f, vertices, indices);	//left face
	ViMesh::MakeQuadRaw(l_t_f, r_t_f, r_t_n, l_t_n, vertices, indices);	//top face
	ViMesh::MakeQuadRaw(r_b_f, l_b_f, l_b_n, r_b_n, vertices, indices);	//bottom face

	ViMesh* mesh = new ViMesh(aMaterial, vertices, indices);

	return mesh;
}