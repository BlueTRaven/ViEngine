#include "Cube.h"

vigame::Cube::Cube(ViMaterial* aMaterial)
{
	CreateMesh(aMaterial);
}

void vigame::Cube::CreateMesh(ViMaterial* aMaterial)
{	
	//<forward/backward - f/b>_<top/bottom - t/b>_<left/right - l/r>

	/*vec3 f_t_l = { -1.0, 1.0, 1.0 };
	vec3 f_t_r = { 1.0, 1.0, 1.0 };
	vec3 f_b_r = { 1.0, -1.0, 1.0 };
	vec3 f_b_l = { -1.0, -1.0, 1.0 };

	vec3 b_t_l = { -1.0, 1.0, -1.0 };
	vec3 b_t_r = { 1.0, 1.0, -1.0 };
	vec3 b_b_r = { 1.0, -1.0, -1.0 };
	vec3 b_b_l = { -1.0, -1.0, -1.0 };

	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;
	ViMesh::MakeQuadRaw(f_t_l, f_t_r, f_b_r, f_b_l, vertices, indices); //front face
	ViMesh::MakeQuadRaw(f_t_r, b_t_r, b_b_r, f_b_r, vertices, indices);	//right face
	ViMesh::MakeQuadRaw(b_t_r, b_t_l, b_b_l, b_b_r, vertices, indices);	//back face
	ViMesh::MakeQuadRaw(b_t_l, f_t_l, f_b_l, b_b_l, vertices, indices);	//left face
	ViMesh::MakeQuadRaw(b_t_l, f_t_l, f_b_l, b_b_l, vertices, indices);	//top face
	ViMesh::MakeQuadRaw(b_b_r, b_b_l, f_b_l, f_b_r, vertices, indices);	//bottom face

	ViMesh* mesh = new ViMesh(aMaterial, vertices, indices);*/

	SetMesh(ViMesh::MakeUCube(aMaterial, { -1.0, 1.0, 1.0 }, { 1.0, -1.0, -1.0 }));
}