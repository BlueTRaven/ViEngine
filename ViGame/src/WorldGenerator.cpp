#include "WorldGenerator.h"

#include "ViEnvironment.h"
#include "ViAssetHandler.h"

#include "VoxelWorld.h"
#include "Chunk.h"

std::random_device* vigame::WorldGenerator::rand = new std::random_device();

void vigame::WorldGenerator::Init(VoxelWorld * aWorld)
{
	mWorld = aWorld;
}

void vigame::WorldGenerator::GenerateChunk(vec3i aChunkWorldPos)
{
	siv::PerlinNoise noise = siv::PerlinNoise();
	
	Chunk* chunk = mWorld->GetChunk(aChunkWorldPos);
	
	int waterLevelHeight = mWorld->GetSize().y - 32;
	int perlinAmp = 8;
	int pixBlendMapHeight = -48;

	ViTexture* tex = ASSET_HANDLER->LoadTexture("circle_gradient");

	//Limit generation on y axis to our chunk
	for (int d = 0; d < Chunk::GetSize().z; d++)
	{
		for (int w = 0; w < Chunk::GetSize().x; w++)
		{
			vec2i index = vec2i(d, w) + vec2i(mWorld->ChunkSpaceToCubeSpace(aChunkWorldPos).x, mWorld->ChunkSpaceToCubeSpace(aChunkWorldPos).z);
			vec2d scaledSize = vec2d(index) / vec2d(Chunk::GetSize().x, Chunk::GetSize().z);

			int x = ((float)index.x / (float)mWorld->GetSize().x) * tex->GetWidth();
			int y = ((float)index.y / (float)mWorld->GetSize().z) * tex->GetHeight();
			vec4i pixel = tex->GetPixel(vec2i(x, y));
			
			double pixPercent = pixel.r / 255.0;
			int pixHeight = (int)((1 - pixPercent) * (double)pixBlendMapHeight);

			double perlin = noise.noise2D(scaledSize.x, scaledSize.y);
			int perlinHeight = (int)(perlin * (double)perlinAmp);

			int heightCube = waterLevelHeight + (perlinHeight + pixHeight);

			for (int h = 0; h < Chunk::GetSize().y; h++)
			{
				vec3i pos = vec3i(index.x, h + mWorld->ChunkSpaceToCubeSpace(aChunkWorldPos).y, index.y);

				if (pos.y < heightCube)
				{
					//int num = RandomInt(rand, 1, 2);
					mWorld->SetCubeInstance(pos, mWorld->GetCubeRegistry()->GetCubeType(1));
				}
				else mWorld->SetCubeInstance(pos, mWorld->GetCubeRegistry()->GetCubeType(0));
			}
		}
	}
}
