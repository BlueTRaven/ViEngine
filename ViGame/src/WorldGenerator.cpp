#include "WorldGenerator.h"

#include "ViEnvironment.h"
#include "ViAssetHandler.h"

#include "VoxelWorld.h"
#include "Chunk.h"
#include "Rand.h"

void vigame::WorldGenerator::Init(VoxelWorld * aWorld)
{
	mWorld = aWorld;
}

void vigame::WorldGenerator::GenerateChunk(Chunk* aChunk)
{
	auto time = std::chrono::steady_clock::now();

	siv::PerlinNoise noise = siv::PerlinNoise();

	int waterLevelHeight = mWorld->GetSize().y - 32;
	int perlinAmp = 8;
	int pixBlendMapHeight = -48;

	ViTexture* tex = ASSET_HANDLER->LoadTexture("circle_gradient");

	//Limit generation on y axis to our chunk
	for (int d = 0; d < Chunk::GetSize().z; d++)
	{
		for (int w = 0; w < Chunk::GetSize().x; w++)
		{
			vec3i cubeSpaceChunkPos = mWorld->ChunkSpaceToCubeSpace(aChunk->GetWorldPosition());
			vec2i index = vec2i(d, w);
			vec2i realPos = index + vec2i(cubeSpaceChunkPos.x, cubeSpaceChunkPos.z);

			int x = ((float)realPos.x / (float)mWorld->GetSize().x) * tex->GetWidth();
			int y = ((float)realPos.y / (float)mWorld->GetSize().z) * tex->GetHeight();
			x = glm::abs(x % tex->GetWidth());
			y = glm::abs(y % tex->GetHeight());
			vec4i pixel = tex->GetPixel(vec2i(x, y));

			double pixPercent = pixel.r / 255.0;
			int pixHeight = (int)((1 - pixPercent) * (double)pixBlendMapHeight);

			//size normalized 0-1
			vec2d nrmSize = vec2d(realPos.x, realPos.y) / vec2d(Chunk::GetSize().x, Chunk::GetSize().z);
			double perlin = noise.noise2D(nrmSize.x, nrmSize.y);
			int perlinHeight = (int)(perlin * (double)perlinAmp);

			//world cube space height of the height map
			int heightCube = waterLevelHeight + (perlinHeight + pixHeight);

			for (int h = 0; h < Chunk::GetSize().y; h++)
			{
				vec3i pos = vec3i(index.x, h, index.y);

				if (h + cubeSpaceChunkPos.y < heightCube)
				{
					int num = vigame::rand::RandInt(1, 3);
					aChunk->SetCubeRelative(mWorld->MakeInstance(num), pos);
				}
				else aChunk->SetCubeRelative(mWorld->MakeInstance((cubeid)0), pos);
			}
		}
	}

	auto endTime = std::chrono::steady_clock::now();
	printf("Debug: Took %f seconds to generate a chunk on thread %i.\n", ((float)std::chrono::duration_cast<std::chrono::milliseconds>(endTime - time).count() / 1000), std::this_thread::get_id());
}
