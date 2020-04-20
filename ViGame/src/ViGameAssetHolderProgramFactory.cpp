#include "ViGameAssetHolderProgramFactory.h"

#include "ProgramCubesInstanced.h"
#include "ProgramLitGeneric.h"

ViProgram* vigame::ViGameAssetHolderProgramFactory::Create(std::string type)
{
	if (type == "cube_instanced")
		return new ProgramCubesInstanced();
	else if (type == "lit_generic")
		return new ProgramLitGeneric();

	return ViAssetHolderProgramFactory::Create(type);
}
