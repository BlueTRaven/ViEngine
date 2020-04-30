#include "ViGameAssetHolderProgramFactory.h"

#include "ProgramCubesInstanced.h"
#include "ProgramLitGeneric.h"
#include "ProgramUnlitGeneric.h"
#include "ProgramOrtho.h"

ViProgram* vigame::ViGameAssetHolderProgramFactory::Create(std::string type)
{
	if (type == "cube_instanced")
		return new ProgramCubesInstanced();
	else if (type == "lit_generic")
		return new ProgramLitGeneric();
	else if (type == "unlit_generic")
		return new ProgramUnlitGeneric();
	else if (type == "ortho")
		return new ProgramOrtho();

	return ViAssetHolderProgramFactory::Create(type);
}
