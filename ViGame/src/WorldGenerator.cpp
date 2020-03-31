#include "WorldGenerator.h"

#include "ViEnvironment.h"
#include "ViAssetHandler.h"

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
	
	int waterLevelHeight = mWorld->GetSize().y - 32;
	int perlinAmp = 8;
	int pixBlendMapHeight = -48;

	ViTexture* tex = ASSET_HANDLER->LoadTexture("circle_gradient");

	//Limit generation on y axis to our chunk
	for (int d = 0; d < Chunk::cDEPTH; d++)
	{
		for (int w = 0; w < Chunk::cWIDTH; w++)
		{
			vec2i index = vec2i(d, w) + vec2i(mWorld->ChunkSpaceToCubeSpace(aChunkPos).x, mWorld->ChunkSpaceToCubeSpace(aChunkPos).z);
			vec2d scaledSize = vec2d((double)index.x / (double)Chunk::cWIDTH, (double)index.y / (double)Chunk::cDEPTH);

			int x = ((float)index.x / (float)mWorld->GetSize().x) * tex->GetWidth();
			int y = ((float)index.y / (float)mWorld->GetSize().z) * tex->GetHeight();
			vec4i pixel = tex->GetPixel(vec2i(x, y));
			
			double pixPercent = pixel.r / 255.0;
			int pixHeight = (int)((1 - pixPercent) * (double)pixBlendMapHeight);

			double perlin = noise.noise2D(scaledSize.x, scaledSize.y);
			int perlinHeight = (int)(perlin * (double)perlinAmp);

			int heightCube = waterLevelHeight + (perlinHeight + pixHeight);

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
