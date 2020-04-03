#pragma once

#include <chrono>
#include <GL/glew.h>
#include <math.h>

#include "ViMaterialFont.h"
#include "ViGame.h"
#include "ProgramCubesInstanced.h"

#include "VoxelWorld.h"
#include "Chunk.h"

#include "Camera.h"
#include "Player.h"

namespace vigame
{
	class VoxelIslandGame : public ViGame
	{
	public:
		VoxelIslandGame();

	protected:
		void Init() override;

		void Update(double aDeltaTime) override;
		void Draw(double aDeltaTime) override;

	private:
		VoxelWorld* world;
		Chunk* highlightedChunk;
		vec3i highlightedCubeInstance;

		Player* player;
		Camera* mCamera;

		ProgramCubesInstanced* mProgramCubesInstanced;

		ViProgramText* mProgramText;

		ViMaterialFont* mDebugFont;

		ViMaterial* genericMaterial;

		ViTransform transform;
		
		bool f;
	};
}