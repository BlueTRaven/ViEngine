#include "Chunk.h"

#include "VoxelWorld.h"

vigame::Chunk::Chunk(vec3i aPosition, VoxelWorld* aWorld) :
	mPosition(aPosition),
	mWorld(aWorld)
{
}

void vigame::Chunk::OptimizeMesh()
{
	std::vector<ViMeshSubsection> subsections;
	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	for (int x = 0; x < cWIDTH; x++)
	{
		for (int y = 0; y < cHEIGHT; y++)
		{
			for (int z = 0; z < cDEPTH; z++)
			{
				vec3i pos = { x, y, z };
				CubeInstance instance = mWorld->GetCubeInstance(pos);

				if (instance.mId == 0)
					continue;

				Cube* cube = mWorld->GetCubeRegistry()->GetCubeType(instance.mId);
				ViTransform worldSpaceTransform = cube->GetWorldSpaceTransform(instance, pos);

				ViMesh* mesh = cube->GetMesh();
				std::vector<ViVertex> meshVertices = mesh->GetVertices();
				std::vector<GLuint> meshIndices = mesh->GetIndices();

				std::vector<int> adjacents = cube->GetSubsectionsToDraw(instance, pos);

				for (int adjacent : adjacents)
				{
					ViMeshSubsection subsection = mesh->GetSubsection(adjacent);

					//need to track this so indices are correct
					int indicesPushStart = indices.size();

					//Get indices, use those to find what vertices they're pointing to
					//Also corrects indices to be accurate in the new mesh
					int start = subsection.start / sizeof(GLuint);
					GLuint startIndex = meshIndices[start];
					for (int i = start; i < start + subsection.size; i++)
					{
						//index of this submesh
						GLuint submIndex = meshIndices[i];
						//vertex that the index points to
						ViVertex submVert = meshVertices[submIndex];
						
						//verticies are, by default, centered on 0, 0, 0, as we use a transform to move them in the vertex shader.
						//This needs to be corrected.
						//submVert.pos = worldSpaceTransform.Matrix() * vec4(submVert.pos.x, submVert.pos.y, submVert.pos.z, 1.0);

						vertices.push_back(submVert);
						indices.push_back(indicesPushStart + (submIndex - startIndex));
					}

					subsections.push_back(ViMeshSubsection(subsection.material, indicesPushStart * sizeof(GLuint), subsection.size));
				}
			}
		}
	}

	mOptimizedMesh = new ViMesh(subsections, vertices, indices);
	mOptimizedMesh->UploadData();
}
