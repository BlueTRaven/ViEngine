#pragma once

#include <vector>
#include <thread>
#include <mutex>

#include "ViUtil.h"

namespace vigame
{
	class Chunk;
	//Manages loading chunks that need to be loaded.
	class ChunkManager
	{
	public:
		enum ChunkManagerState 
		{
			cUNSTARTED,
			cSTARTED,
			cPAUSED
		};

		ChunkManager();

		//Starts the chunk manager. Does nothing if already started, and re-starts the chunk manager if paused. Thread-safe.
		void Start();

		//Pauses the chunk manager. It will no longer load new chunks. Call Start to re-start it. Thread-safe.
		void Pause();

		//Thread-safe. Enqueues a chunk to be loaded.
		void AddChunkToLoad(Chunk* aChunk);

		//Thread-safe. Gets a list of all chunks that have finished loading.
		std::vector<Chunk*> GetFinishedChunks();

		//Thread-safe
		void SortChunksByDistance(vec3i aChunkPoint);

		vi_property_get_named(ChunkManagerState, mState, State);

	private:
		void Run();

		bool CanGenerateChunk(Chunk* aChunk);

		std::vector<Chunk*> mChunksToLoad;
		std::vector<Chunk*> mFinishedChunks;

		std::thread* mThread;
		std::mutex* mAddChunksMutex;
		std::mutex* mAddFinishedChunksMutex;

		ChunkManagerState mState;
	};
}