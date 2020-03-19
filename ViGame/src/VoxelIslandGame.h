#pragma once

#include "GL/glew.h"
#include "ViGame.h"

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
		ViProgramGeneric* mProgramGeneric;
		ViProgramText* mProgramText;
		ViFont* testFont;
		ViMaterial* genericMaterial;
		ViMaterial* textMaterial;

		ViTransform transform;
	};
}