#include "ViGameAssetHolderProgramFactory.h"

#include "ProgramCubesInstanced.h"

ViProgram* vigame::ViGameAssetHolderProgramFactory::Create(std::string type)
{
	if (type == "cube_instanced")
		return new ProgramCubesInstanced();

	return ViAssetHolderProgramFactory::Create(type);
}
