#include "WorldGenerator.h"

#include "VoxelWorld.h"
#include "Chunk.h"

void vigame::WorldGenerator::Init(VoxelWorld * aWorld)
{
	mWorld = aWorld;
}

void vigame::WorldGenerator::GenerateChunk(vec3i aChunkPos)
{
	siv::PerlinNoise noise = siv::PerlinNoise();
	
	Chunk* chunk = mWorld->GetChunk(aChunkPos);
	
	int startHeight = mWorld->GetSize().y - 32;
	int maxHeight = 8;	//amplitude, in other words

	//Limit generation on y axis to our chunk
	for (int d = 0; d < Chunk::cDEPTH; d++)
	{
		for (int w = 0; w < Chunk::cWIDTH; w++)
		{
			vec2i index = vec2i(d, w) + vec2i(mWorld->ChunkSpaceToCubeSpace(aChunkPos).x, mWorld->ChunkSpaceToCubeSpace(aChunkPos).z);
			vec2d scaledSize = vec2d((double)index.x / (double)Chunk::cWIDTH, (double)index.y / (double)Chunk::cDEPTH);

			double out = noise.noise2D(scaledSize.x, scaledSize.y);//noise.accumulatedOctaveNoise2D(scaledSize.x, scaledSize.y, 2);

			int worldSpaceOut = (int)(out * (double)(maxHeight));

			int heightCube = startHeight + worldSpaceOut;

			for (int h = 0; h < Chunk::cHEIGHT; h++)
			{
				vec3i pos = vec3i(index.x, h + mWorld->ChunkSpaceToCubeSpace(aChunkPos).y, index.y);

				if (pos.y < heightCube)
					mWorld->SetCubeInstance(pos, mWorld->GetCubeRegistry()->GetCubeType(1));
				else mWorld->SetCubeInstance(pos, mWorld->GetCubeRegistry()->GetCubeType(0));
			}
		}
	}
}
