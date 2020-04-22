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

		//Thread-safe.
		void AddChunkToLoad(Chunk* aChunk);

		//Thread-safe.
		std::vector<Chunk*> GetFinishedChunks();

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