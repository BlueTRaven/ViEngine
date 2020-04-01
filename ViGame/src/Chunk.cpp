#include "Chunk.h"

#include <exception>

#include "ViVertexBatch.h"
#include "VoxelWorld.h"
#include "CubeInstance.h"
#include "ViColorsGL.h"

vigame::Chunk::Chunk(vec3i aPosition, VoxelWorld* aWorld) :
	mPosition(aPosition),
	mWorld(aWorld),
	mDirty(false),
	mOptimizedMesh(nullptr),
	mOtherMesh(nullptr),
	mHasAnything(true),
	meshingThread(nullptr),
	mut(new std::mutex())
{
	vec3 pos = vec3(aPosition) * aWorld->GetGridSize() * vec3(cWIDTH, cHEIGHT, cDEPTH) - (aWorld->GetGridSize() / 2);
	vec3 size = aWorld->GetGridSize() * vec3(cWIDTH, cHEIGHT, cDEPTH);
	mWireframeMesh = ViMesh::MakeUCube(ASSET_HANDLER->LoadMaterial("white_pixel"), pos, pos + size, ViMesh::cFACE_ALL, vicolors::WHITE);
}

vigame::Chunk::~Chunk()
{
	if (mOptimizedMesh != nullptr)
		delete mOptimizedMesh;	

	delete mWireframeMesh;
}

void vigame::Chunk::Draw(ViVertexBatch* aBatch)
{
	TryMeshing();

	if (mut->try_lock())
	{
		if (!meshing && meshingThread->joinable())
		{
			meshingThread->join();

			//UploadData must be called on main thread
			//Sometimes we generate a null mesh; in this case we don't want to try uploading anything.
			if (mOptimizedMesh != nullptr)
				mOptimizedMesh->UploadData();
		}

		if (mHasAnything && mOptimizedMesh)
			aBatch->Draw(ViTransform::Positioned(vec3(mPosition) * mWorld->GetGridSize() * vec3(cWIDTH, cHEIGHT, cDEPTH)), mOptimizedMesh);
		//aBatch->Draw(ViTransform::Positioned(vec3(0)), optimizedMesh);
		mut->unlock();
	}
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

void vigame::Chunk::NotifyCubeChanged(vec3i aPosition, CubeInstance& aPreviousCubeInstance, CubeInstance& aCubeInstance)
{
}

void vigame::Chunk::TryMeshing()
{
	if (!meshing && mHasAnything && (mOptimizedMesh == nullptr || GetDirty()))
	{
		if (mut->try_lock())
		{
			if (meshingThread != nullptr && meshingThread->joinable())
			{
				meshingThread->join();
			}

			//If our mesh has changed at all, we want to get rid of the old one. 
			if (mOptimizedMesh != nullptr)
				delete mOptimizedMesh;

			mut->unlock();

			meshing = true;
			meshingThread = new std::thread(&Chunk::GreedyMesh, this);
			printf("Debug: Starting chunk thread %i.\n", meshingThread->get_id());

			SetDirty(false);
		}
	}
}

void vigame::Chunk::GreedyMesh()
{
	//lock our mutex so we can't use it in the main drawing thread
	mut->lock();
	meshing = true;

	auto time = std::chrono::steady_clock::now();

	std::vector<ViMeshSubsection> subsections;
	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	int size[] = { cWIDTH, cHEIGHT, cDEPTH };

	int start = indices.size();

	for (size_t d = 0; d < 3; ++d)
	{
		int w = 0;
		int h = 0;
		int k = 0;
		int l = 0;
		int u = (d + 1) % 3;	//the other 2 axis
		int v = (d + 2) % 3;
		int x[] = { 0, 0, 0 };
		int q[] = { 0, 0, 0 };

		std::vector<uint32_t> mask;
		mask.resize(size[u] * size[v]);

		q[d] = 1;

		for (x[d] = -1; x[d] < size[d];)
		{
			int n = 0;

			for (x[v] = 0; x[v] < size[v]; ++x[v])
			{
				for (x[u] = 0; x[u] < size[u]; ++x[u])
				{
					bool curr = 0 <= x[d] ? GetCube(vec3i(x[0], x[1], x[2])).mId != 0 : false;
					bool other = x[d] < size[d] - 1 ? GetCube(vec3i(x[0], x[1], x[2]) + vec3i(q[0], q[1], q[2])).mId != 0 : false;
					bool isBehind = curr && !other;
					cubeid id = curr ? GetCube(vec3i(x[0], x[1], x[2])).mId  : GetCube(vec3i(x[0], x[1], x[2]) + vec3i(q[0], q[1], q[2])).mId;
					//bits are like so: 
					//uuuuuuuuuuuuuuuuuuuuccccccccuubi
					//where:
					//u is unused
					//c is cubeid
					//b is forward/backward
					//i is used or not
					if (curr != other)
						mask[n++] = curr != other | ((uint32_t)isBehind << 1) | ((uint32_t)id << 4);
					else mask[n++] = 0;
				}
			}

			++x[d];

			n = 0;

			for (int j = 0; j < size[v]; ++j)
			{
				for (int i = 0; i < size[u];)
				{
					if (mask[n])
					{
						//Loop along the width until we find something that isn't mask
						w = 1;
						while (i + w < size[u] && mask[n + w])
							++w;

						bool done = false;

						//Loop along the height and width until we find something that isn't mask
						for (h = 1; j + h < size[v]; ++h)
						{
							for (k = 0; k < w; ++k)
							{
								if (!mask[n + k + h * size[u]])
								{
									done = true;
									break;
								}
							}

							if (done)
								break;
						}

						x[u] = i;
						x[v] = j;

						vec3 xv = vec3i(x[0], x[1], x[2]);
						vec3 du = vec3i(0);
						vec3 dv = vec3i(0);
						du[u] = w;
						dv[v] = h;

						vec3 startPos = vec3(mPosition);
						xv = xv * mWorld->GetGridSize();
						du = du * mWorld->GetGridSize();
						dv = dv * mWorld->GetGridSize();

						vec3 p0 = xv;
						vec3 p1 = xv + du;
						vec3 p2 = xv + du + dv;
						vec3 p3 = xv + dv;

						if (mask[n] & 0b10)
						{
							vec3 nrm = glm::cross(p1 - p0, p2 - p0);

							ViMesh::MakeQuadRaw(
								p3,
								p2,
								p1,
								p0,
								nrm, vertices, indices, vicolors::WHITE);
						}
						else
						{
							vec3 nrm = glm::cross(p2 - p0, p1 - p0);

							ViMesh::MakeQuadRaw(
								p0,
								p1,
								p2,
								p3,
								nrm, vertices, indices, vicolors::WHITE);
						}

						for (l = 0; l < h; ++l)
						{
							for (k = 0; k < w; ++k)
							{
								mask[n + k + l * size[u]] = 0;
							}
						}

						i += w;
						n += w;
					}
					else
					{
						++i;
						++n;
					}
				}
			}
		}
	}

	if (vertices.size() == 0 || indices.size() == 0)
	{
		mHasAnything = false;

		meshing = false;
		mut->unlock();
		return;
	}

	mOptimizedMesh = new ViMesh(ASSET_HANDLER->LoadMaterial("white_pixel"), vertices, indices);

	auto endTime = std::chrono::steady_clock::now();
	printf("Debug: Took %f seconds to generate a chunk on thread %i.\n", ((float)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time).count() / 1000), std::this_thread::get_id());

	meshing = false;
	mut->unlock();
}

vigame::CubeInstance vigame::Chunk::GetCube(vec3i aPosition)
{
	return mWorld->GetCubeInstance(aPosition + mWorld->ChunkSpaceToCubeSpace(mPosition));
}
