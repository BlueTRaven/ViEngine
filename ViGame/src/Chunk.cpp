#include "Chunk.h"

vigame::Chunk::Chunk(vec3i aPosition, VoxelWorld* aWorld) :
	mPosition(aPosition),
	mWorld(aWorld)
{
}

void vigame::Chunk::OptimizeMesh()
{
	for (int x = 0; x < cWIDTH; x++)
	{
		for (int y = 0; y < cHEIGHT; y++)
		{
			for (int z = 0; z < cDEPTH; z++)
			{

			}
		}
	}
}
