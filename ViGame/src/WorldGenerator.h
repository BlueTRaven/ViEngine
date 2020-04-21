#pragma once

#include <random>

#include "ViUtil.h"
#include "PerlinNoise.h"

namespace vigame
{
	class VoxelWorld;
	class Chunk;

	class WorldGenerator
	{
	public:
		void Init(VoxelWorld* aWorld);

		void GenerateChunk(Chunk* aChunk);

	private:
		vi_property_get_named(VoxelWorld*, mWorld, World);
		VoxelWorld* mWorld;

		static std::random_device* rand;

		void ThreadedGenerateChunk(Chunk* aChunk);
	};
}