#include "ChunkManager.h"

#include "Chunk.h"

vigame::ChunkManager::ChunkManager() :
	mState(cUNSTARTED),
	mThread(nullptr),
	mAddChunksMutex(new std::mutex),
	mAddFinishedChunksMutex(new std::mutex)
{
}

void vigame::ChunkManager::Start()
{
	if (mState == cUNSTARTED)
	{
		mThread = new std::thread(&ChunkManager::Run, this);
		mState = cSTARTED;
	}
	else if (mState == cPAUSED)
		mState = cSTARTED;
	//Does nothing if already started
}

void vigame::ChunkManager::Pause()
{
	if (mState == cUNSTARTED)
	{
		printf("Error: cannot pause unstarted chunk manager.");
		return;
	}

	mState = cPAUSED;
}

void vigame::ChunkManager::AddChunkToLoad(Chunk * aChunk)
{
	mAddChunksMutex->lock();
	mChunksToLoad.push_back(aChunk);
	mAddChunksMutex->unlock();
}

std::vector<vigame::Chunk*> vigame::ChunkManager::GetFinishedChunks()
{
	mAddFinishedChunksMutex->lock();
	std::vector<Chunk*> copyFinishedChunks = mFinishedChunks;
	mFinishedChunks.clear();
	mAddFinishedChunksMutex->unlock();

	return copyFinishedChunks;
}

void vigame::ChunkManager::SortChunksByDistance(vec3i aChunkPoint)
{
	mAddChunksMutex->lock();
	std::sort(mChunksToLoad.begin(), mChunksToLoad.end(), [=](Chunk* a, Chunk* b) 
	{
		vec3i distA = a->GetWorldPosition() - aChunkPoint;
		vec3i distB = b->GetWorldPosition() - aChunkPoint;
		return distA.length() > distB.length();
	});
	mAddChunksMutex->unlock();
}

void vigame::ChunkManager::Run()
{
	while (true)
	{
		if (mState == cPAUSED)
			std::this_thread::sleep_for(std::chrono::milliseconds(100));

		mAddChunksMutex->lock();
		std::vector<Chunk*> copyChunksToLoad = mChunksToLoad;
		mAddChunksMutex->unlock();

		while (mChunksToLoad.size() > 0)
		{
			mAddChunksMutex->lock();
			Chunk* chunk = mChunksToLoad.back();

			if (CanGenerateChunk(chunk))
			{
				mChunksToLoad.pop_back();
				mAddChunksMutex->unlock();

				chunk->Load(false, true);

				mAddFinishedChunksMutex->lock();
				mFinishedChunks.push_back(chunk);
				mAddFinishedChunksMutex->unlock();
			}
			else mAddChunksMutex->unlock();

			if (mState == cPAUSED)
				break;
		}
	}
}

bool vigame::ChunkManager::CanGenerateChunk(Chunk * aChunk)
{
	return true;
}
