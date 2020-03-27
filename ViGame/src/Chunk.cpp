#include "Chunk.h"

#include "ViVertexBatch.h"
#include "VoxelWorld.h"

/*const int vigame::Chunk::cWIDTH = 16;
const int vigame::Chunk::cHEIGHT = 16;
const int vigame::Chunk::cDEPTH = 16;*/

vigame::Chunk::Chunk(vec3i aPosition, VoxelWorld* aWorld) :
	mPosition(aPosition),
	mWorld(aWorld),
	mDirty(false),
	mOptimizedMesh(nullptr)
{
}

vigame::Chunk::~Chunk()
{
	if (mOptimizedMesh != nullptr)
		delete mOptimizedMesh;	
}

void vigame::Chunk::Draw(ViVertexBatch* aBatch)
{
	ViMesh* optimizedMesh = GetOptimizedMesh();
	if (optimizedMesh == nullptr || GetDirty())
	{
		if (optimizedMesh != nullptr)
			delete optimizedMesh;

		OptimizeMesh();
		optimizedMesh = GetOptimizedMesh();

		SetDirty(false);
	}

	aBatch->Draw(ViTransform::Positioned({ 0, 0, 0 }), optimizedMesh);
}

void vigame::Chunk::OptimizeMesh()
{
	//TODO optimize dirty calls
	//Right now, any time any cube within the chunk is changed, this entire thing is called again.
	//Perhaps we could have some deterministic mapping back and forth so we could easily remove vertices from the middle?
	//Linked lists? Vertex data would only be sent once, and linked lists are good for removing stuff from the middle of them
	//Regardless, everything has to be re-uploaded every time
	struct OptimizedMeshSubsection 
	{
		ViMesh* mesh;
		ViMeshSubsection subs;
		ViTransform trans;
	};

	std::vector<ViMeshSubsection> subsections;
	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	std::vector<OptimizedMeshSubsection> toSort;

	int cachedCubeType = 0;
	Cube* cachedCube;
	std::vector<int> cachedAdjacents;

	for (int z = 0; z < cDEPTH; z++)
	{
		for (int y = 0; y < cHEIGHT; y++)
		{
			for (int x = 0; x < cWIDTH; x++)
			{
				vec3i pos = vec3i(x, y, z) + mWorld->ChunkSpaceToCubeSpace(mPosition);
				CubeInstance instance = mWorld->GetCubeInstance(pos);

				if (instance.mId == 0)
					continue;

				//if (cachedCubeType == 0 || cachedCubeType != instance.mId)
				{
					cachedCubeType = instance.mId;
					cachedCube = mWorld->GetCubeRegistry()->GetCubeType(instance.mId);
					cachedAdjacents = cachedCube->GetSubsectionsToDraw(instance, pos);
				}

				for (int adjacent : cachedAdjacents)
				{
					ViMeshSubsection subsection = cachedCube->GetMesh()->GetSubsection(adjacent);

					OptimizedMeshSubsection subs = OptimizedMeshSubsection();
					subs.mesh = cachedCube->GetMesh();
					subs.subs = subsection;
					subs.trans = cachedCube->GetWorldSpaceTransform(instance, pos);
					toSort.push_back(subs);
				}
			}
		}
	}

	if (toSort.size() == 0)
	{
		mOptimizedMesh = ViMesh::GetEmpty();
		return;
	}

	//sort them so that same materials are beside each other
	//Note that it doesn't matter what order the vertices are in; we're rebuilding the mesh from scratch anyway
	std::sort(toSort.begin(), toSort.end(), [](OptimizedMeshSubsection a, OptimizedMeshSubsection b)
	{
		return a.subs.material < b.subs.material;
	});

	OptimizedMeshSubsection oldVal = OptimizedMeshSubsection();
	int count = 0;
	int indicesStart = 0;
	bool first = true;

	for (OptimizedMeshSubsection val : toSort)
	{
		bool materialChanged = !first && oldVal.subs.material != val.subs.material;

		if (materialChanged)
		{
			subsections.push_back(ViMeshSubsection(oldVal.subs.material, indicesStart, count));
			count = 0;

			indicesStart = indices.size();
		}
		
		int pushStart = vertices.size();

		GLuint startIndex = val.mesh->GetIndices()[val.subs.start];

		std::vector<ViVertex> meshVertices = val.mesh->GetVertices();
		mat4 mat = val.trans.Matrix();

		for (int i = 0; i < 4; i++)
		{
			ViVertex vert = meshVertices[startIndex + i];
			vert.pos = mat * vec4(vert.pos.x, vert.pos.y, vert.pos.z, 1.0);

			vertices.push_back(vert);
		}

		indices.push_back(pushStart + 0);
		indices.push_back(pushStart + 1);
		indices.push_back(pushStart + 3);
		indices.push_back(pushStart + 1);
		indices.push_back(pushStart + 2);
		indices.push_back(pushStart + 3);

		count += 6;

		oldVal = val;
		first = false;
	}

	subsections.push_back(ViMeshSubsection(oldVal.subs.material, indicesStart, count));

	mOptimizedMesh = new ViMesh(subsections, vertices, indices);
	mOptimizedMesh->UploadData();
}
