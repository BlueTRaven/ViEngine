#pragma once

#include <chrono>
#include <GL/glew.h>
#include <math.h>

#include "ViGame.h"

#include "VoxelWorld.h"
#include "Chunk.h"

namespace vigame
{
	class VoxelIslandGame : public ViGame
	{
	public:
		VoxelIslandGame();

	protected:
		void Init() override;

		void Update() override;
		void Draw() override;

	private:
		VoxelWorld* world;

		Chunk* mTestChunk;

		ViProgramGeneric* mProgramGeneric;
		ViProgramText* mProgramText;
		ViFont* testFont;
		ViMaterial* genericMaterial;
		ViMaterial* textMaterial;

		ViTransform transform;
	};
}