#pragma once

#include <thread>
#include <functional>

class ViMesh;

namespace vigame
{
	class Chunk;
	class WorldGenerator;

	//Manages loading chunks.
	class ChunkLoader
	{
	public:
		ChunkLoader(Chunk* aChunk, std::function<ViMesh*(void)> aMeshingCallback, WorldGenerator* aWorldGenerator);

		//Call on main thread
		//Param aGenerate: whether the chunk should be generated (or regenerated.) if true, all cubes are memset to zero, then generated.
		//Returns the old mesh if available, otherwise nullptr.
		ViMesh* StartThreaded(bool aGenerate);

		ViMesh* Start(bool aGenerate);

		//Call on main thread
		//Uploads mesh data and returns the mesh
		ViMesh* Finish();

		inline bool IsFinished() const
		{
			return mFinished;
		}

	private:
		Chunk* mChunk;
		std::function<ViMesh*(void)> mMeshingCallback;
		WorldGenerator* mWorldGenerator;

		std::thread* mThread;

		ViMesh* mMesh;
		bool mFinished;

		void Run(bool aGenerate);
	};
}