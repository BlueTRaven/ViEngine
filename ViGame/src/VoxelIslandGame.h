#pragma once

#include <chrono>
#include <GL/glew.h>

#include "ViGame.h"
#include "VoxelWorld.h"

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

		ViProgramGeneric* mProgramGeneric;
		ViProgramText* mProgramText;
		ViFont* testFont;
		ViMaterial* genericMaterial;
		ViMaterial* textMaterial;

		ViTransform transform;
	};
}