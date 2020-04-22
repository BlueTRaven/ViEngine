#include "Chunk.h"

#include <exception>

#include "ViVertexBatch.h"
#include "VoxelWorld.h"
#include "CubeInstance.h"
#include "ViColorsGL.h"
#include "WorldGenerator.h"

vigame::Chunk::Chunk(vec3i aWorldPosition, VoxelWorld* aWorld) :
	mWorldPosition(aWorldPosition),
	mCubes((CubeInstance*)malloc((mSize.x * mSize.y * mSize.z) * sizeof(CubeInstance))),
	mWorld(aWorld),
	mDirty(false),
	mOptimizedMesh(nullptr),
	mOldOptimizedMesh(nullptr),
	mHasAnything(true),
	mChunkState(cUNINIT),
	mGenerated(false)
{
	//set all ids to 0
	memset(mCubes, 0, mSize.x * mSize.y * mSize.z * sizeof(CubeInstance));

	vec3 pos = vec3(aWorldPosition) * aWorld->GetGridSize() * vec3(GetSize()) - (aWorld->GetGridSize() / 2);
	vec3 size = aWorld->GetGridSize() * vec3(GetSize());

	std::function<ViMesh*(void)> meshingFunction = nullptr;

	switch (mMeshingMethod)
	{
	case cSTUPID:
		meshingFunction = nullptr;
		break;
	case cNAIVE:
		meshingFunction = std::bind(&Chunk::NaiveMesh, this);
		break;
	case cGREEDY:
		meshingFunction = std::bind(&Chunk::GreedyMesh, this);
		break;
	default:
		break;
	}

	mChunkLoader = new ChunkLoader(this, meshingFunction, mWorld->GetGenerator());
}

vigame::Chunk::~Chunk()
{
	if (mOptimizedMesh != nullptr)
		delete mOptimizedMesh;	

	if (mOldOptimizedMesh != nullptr)
		delete mOldOptimizedMesh;

	delete mChunkLoader;

	free(mCubes);
}

vec3i vigame::Chunk::mSize = vec3i(32, 64, 32);

void vigame::Chunk::SetSize(vec3i aSize)
{
	mSize = aSize;
}

vec3i vigame::Chunk::GetSize()
{
	return mSize;
}

vigame::Chunk::MeshingMethod vigame::Chunk::mMeshingMethod = vigame::Chunk::MeshingMethod::cNAIVE;

void vigame::Chunk::SetMeshingMethod(MeshingMethod aMeshingMethod)
{
	mMeshingMethod = aMeshingMethod;
}

vigame::Chunk::MeshingMethod vigame::Chunk::GetMeshingMethod()
{
	return mMeshingMethod;
}

void vigame::Chunk::Load(bool aMultiThread, bool aGenerate)
{
	if (aMultiThread)
		mOldOptimizedMesh = mChunkLoader->StartThreaded(aGenerate);
	else mOldOptimizedMesh = mChunkLoader->Start(aGenerate);

	mChunkState = ChunkState::cLOADING;
}

void vigame::Chunk::Draw(ViVertexBatch* aBatch)
{
	/*if (mChunkState == ChunkState::cUNINIT)
	{
		Load(true, true);
	}*/

	if (mChunkLoader->IsFinished() && mChunkState == ChunkState::cLOADING)
	{
		mChunkState = ChunkState::cDONE;

		mOptimizedMesh = mChunkLoader->Finish();

		if (mOldOptimizedMesh != nullptr)
		{
			delete mOldOptimizedMesh;
			mOldOptimizedMesh = nullptr;
		}
	}

	if (mChunkState == ChunkState::cDONE)
	{
		if (mHasAnything)
		{
			if (mOptimizedMesh)
				aBatch->Draw(ViTransform::Positioned(vec3(mWorldPosition) * mWorld->GetGridSize() * vec3(GetSize())), mOptimizedMesh);
		}
	}
	else if (mHasAnything && mOldOptimizedMesh)
	{
		//If we cannot lock the mutex, try to draw the old optimized mesh, if it exists yet.
		//(If the mutex is locked, that means we're still meshing.)
		aBatch->Draw(ViTransform::Positioned(vec3(mWorldPosition) * mWorld->GetGridSize() * vec3(GetSize())), mOldOptimizedMesh);
	}
}

void vigame::Chunk::MakeMesh(MeshingMethod aMethod)
{
	/*if (mHasAnything)
	{
		if (mut->try_lock())
		{
			if (mMeshingThread != nullptr && mMeshingThread->joinable())
			{
				mMeshingThread->join();
			}

			//If our mesh has changed at all, we want to get rid of the old one. 
			//However, we need to hold onto the old mesh so we can draw it until the new one is generated.
			if (mOptimizedMesh != nullptr)
			{
				if (mOldOptimizedMesh != nullptr)
				{
					delete mOldOptimizedMesh;
					mOldOptimizedMesh = nullptr;
				}

				mOldOptimizedMesh = new ViMesh(*mOptimizedMesh);
				delete mOptimizedMesh;
				mOptimizedMesh = nullptr;
			}

			mut->unlock();

			switch (aMethod)
			{
			case cSTUPID:
				SetChunkState(cUNINIT);
				printf("Debug: STUPID chunk meshing method is not implemented! Use NAIVE or GREEDY.");
				break;	
			case cNAIVE:
				mMeshingThread = new std::thread(&Chunk::NaiveMesh, this);
				printf("Debug: Starting NAIVE chunk thread %i.\n", mMeshingThread->get_id());
				break;
			case cGREEDY:
				mMeshingThread = new std::thread(&Chunk::GreedyMesh, this);
				printf("Debug: Starting GREEDY chunk thread %i.\n", mMeshingThread->get_id());
				break;
			default:
				break;
			}

			SetDirty(false);
		}
	}*/
}

ViMesh* vigame::Chunk::NaiveMesh()
{
	auto time = std::chrono::steady_clock::now();

	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	for (int z = 0; z < mSize.z; z++)
	{
		for (int y = 0; y < mSize.y; y++)
		{
			for (int x = 0; x < mSize.x; x++)
			{
				vec3i pos(x, y, z);
				//vec3i realPos = pos + (mWorldPosition * GetSize());
				CubeInstance cube = GetCubeRelative(pos);

				if (cube.mId == 0)
					continue;

				Cube* c = mWorld->GetCubeRegistry()->GetCubeType(cube.mId);
				int adjacents = 0;

				if (GetCubePotentially(pos + vec3i(-1, 0, 0)).mId == 0)
					adjacents = adjacents | ViMesh::cFACE_LEFT;
				if (GetCubePotentially(pos + vec3i(1, 0, 0)).mId == 0)
					adjacents = adjacents | ViMesh::cFACE_RIGHT;
				if (GetCubePotentially(pos + vec3i(0, -1, 0)).mId == 0)
					adjacents = adjacents | ViMesh::cFACE_TOP;
				if (GetCubePotentially(pos + vec3i(0, 1, 0)).mId == 0)
					adjacents = adjacents | ViMesh::cFACE_BOTTOM;
				if (GetCubePotentially(pos + vec3i(0, 0, -1)).mId == 0)
					adjacents = adjacents | ViMesh::cFACE_FRONT;
				if (GetCubePotentially(pos + vec3i(0, 0, 1)).mId == 0)
					adjacents = adjacents | ViMesh::cFACE_BACK;

				if (adjacents == 0)
					continue;

				ViMesh::MakeUCubeRaw(vec3(pos) * mWorld->GetGridSize(), (vec3(pos) * mWorld->GetGridSize()) + mWorld->GetGridSize(), adjacents, vertices, indices, 
					mWorld->GetCubeRegistry()->GetCubeType(cube.mId)->GetColor());
			}
		}
	}

	if (vertices.size() == 0)
	{
		mHasAnything = false;

		auto endTime = std::chrono::steady_clock::now();
		printf("Debug: Took %f seconds to mesh a chunk on thread %i.\n", ((float)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time).count() / 1000), std::this_thread::get_id());

		return nullptr;
	}

	auto endTime = std::chrono::steady_clock::now();
	printf("Debug: Took %f seconds to mesh a chunk on thread %i.\n", ((float)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time).count() / 1000), std::this_thread::get_id());

	return new ViMesh(ASSET_HANDLER->LoadMaterial("white_pixel"), vertices, indices);
}

ViMesh* vigame::Chunk::GreedyMesh()
{
	auto time = std::chrono::steady_clock::now();

	std::vector<ViMeshSubsection> subsections;
	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	int size[] = { mSize.x, mSize.y, mSize.z };

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

			std::set<cubeid> ids;
			//std::vector<cubeid> ids;

			for (x[v] = 0; x[v] < size[v]; ++x[v])
			{
				for (x[u] = 0; x[u] < size[u]; ++x[u])
				{
					cubeid curr = 0 <= x[d] ? GetCubeRelative(vec3i(x[0], x[1], x[2])).mId : false;
					cubeid other = x[d] < size[d] - 1 ? GetCubeRelative(vec3i(x[0], x[1], x[2]) + vec3i(q[0], q[1], q[2])).mId : false;
					bool isBehind = curr && other == 0;
					//bits are like so: 
					//uuuuuuuuuuuuuuuuuuuuccccccccuubi
					//where:
					//u is unused
					//c is cubeid
					//b is forward/backward
					//i is used or not
					if ((curr == 0 && other != 0) || (other == 0 && curr != 0))
					{
						mask[n++] = curr != other | ((uint32_t)isBehind << 1) | ((uint32_t)curr << 4) | ((uint32_t)other << 4 + (int)(sizeof(cubeid) * 8));
						
						//Check to see if this type exists already
						if (ids.find(curr) == ids.end())
							ids.emplace(curr);
						if (ids.find(other) == ids.end())
							ids.emplace(other);
						/*//TODO: will set work better here?
						bool found = false;
						for (auto iter = ids.begin(); iter != ids.end(); iter++)
						{
							if (*iter == id)
							{
								found = true;
								break;
							}
						}

						if (!found)
							ids.push_back(id);*/
					}
					else mask[n++] = 0;
				}
			}

			++x[d];

			for (auto iter = ids.begin(); iter != ids.end(); iter++)
			{
				n = 0;

				for (int j = 0; j < size[v]; ++j)
				{
					for (int i = 0; i < size[u];)
					{
						if (mask[n])
						{
							cubeid currId = mask[n] >> 4;
							cubeid otherId = mask[n] >> 4 + (int)(sizeof(cubeid) * 8);
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

							vec3 startPos = vec3(mWorldPosition);
							xv = xv * mWorld->GetGridSize();
							du = du * mWorld->GetGridSize();
							dv = dv * mWorld->GetGridSize();

							vec3 p0 = xv;
							vec3 p1 = xv + du;
							vec3 p2 = xv + du + dv;
							vec3 p3 = xv + dv;

							if (mask[n] & 0b10)
							{
								vec3 nrm = glm::normalize(glm::cross(p1 - p0, p2 - p0));

								ViMesh::MakeQuadRaw(
									p3,
									p2,
									p1,
									p0,
									nrm, vertices, indices, mWorld->GetCubeRegistry()->GetCubeType(currId)->GetColor());
							}
							else
							{
								vec3 nrm = glm::normalize(glm::cross(p2 - p0, p1 - p0));

								ViMesh::MakeQuadRaw(
									p0,
									p1,
									p2,
									p3,
									nrm, vertices, indices, mWorld->GetCubeRegistry()->GetCubeType(currId)->GetColor());
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
	}

	if (vertices.size() == 0 || indices.size() == 0)
	{
		mHasAnything = false;

		auto endTime = std::chrono::steady_clock::now();
		printf("Debug: Took %f seconds to mesh a chunk on thread %i.\n", ((float)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time).count() / 1000), std::this_thread::get_id());

		return nullptr;
	}

	auto endTime = std::chrono::steady_clock::now();
	printf("Debug: Took %f seconds to generate a chunk on thread %i.\n", ((float)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time).count() / 1000), std::this_thread::get_id());

	return new ViMesh(ASSET_HANDLER->LoadMaterial("white_pixel"), vertices, indices);
}

vigame::CubeInstance vigame::Chunk::GetCubeRelative(vec3i aPosition)
{
	if (aPosition.x < 0 || aPosition.y < 0 || aPosition.z < 0 || aPosition.x >= mSize.x || aPosition.y >= mSize.y || aPosition.z >= mSize.z)
		return CubeInstance(0);

	return mCubes[Vec3IndexToIndex(aPosition, mSize)];
}

void vigame::Chunk::SetCubeRelative(CubeInstance instance, vec3i aPosition)
{
	if (aPosition.x < 0 || aPosition.y < 0 || aPosition.z < 0 || aPosition.x >= mSize.x || aPosition.y >= mSize.y || aPosition.z >= mSize.z)
		return;

	if (instance.mId > 0)
		mHasAnything = true;

	mCubes[Vec3IndexToIndex(aPosition, mSize)] = instance;

	SetDirty(true);
}

vigame::CubeInstance vigame::Chunk::GetCubePotentially(vec3i aPosition)
{
	if (aPosition.x < 0 || aPosition.y < 0 || aPosition.z < 0 || aPosition.x >= mSize.x || aPosition.y >= mSize.y || aPosition.z >= mSize.z)
		return mWorld->GetCube(aPosition + (mWorldPosition * GetSize()));

	return GetCubeRelative(aPosition);
}

bool vigame::Chunk::CanDelete()
{
	return mChunkState == cDONE || mChunkState == cUNINIT;
}
