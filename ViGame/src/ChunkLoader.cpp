#include "ChunkLoader.h"

#include "Chunk.h"
#include "ViMesh.h"
#include "WorldGenerator.h"

vigame::ChunkLoader::ChunkLoader(Chunk* aChunk, std::function<ViMesh*(void)> aMeshingCallback, WorldGenerator* aWorldGenerator) :
	mChunk(aChunk),
	mMeshingCallback(aMeshingCallback),
	mWorldGenerator(aWorldGenerator),
	mThread(nullptr),
	mFinished(false),
	mMesh(nullptr)
{
}

ViMesh* vigame::ChunkLoader::StartThreaded(bool aGenerate)
{
	//Assignment in case starting the thread somehow finishes before returning does
	ViMesh* mOldMesh = mMesh;

	mThread = new std::thread(&ChunkLoader::Run, this, aGenerate);

	return mOldMesh;
}

ViMesh* vigame::ChunkLoader::Finish()
{
	mThread->join();
	delete mThread;
	mThread = nullptr;

	if (mMesh != nullptr)
		mMesh->UploadData();

	return mMesh;
}

void vigame::ChunkLoader::Run(bool aGenerate)
{
	mFinished = false;

	if (aGenerate)
		mWorldGenerator->GenerateChunk(mChunk);

	mMesh = mMeshingCallback();

	mFinished = true;
}