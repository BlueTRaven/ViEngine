#include "ViAssetHolderProgramFactory.h"

#include "ProgramGeneric.h"
#include "ProgramText.h"

ViProgram* ViAssetHolderProgramFactory::Create(std::string type)
{
	if (type == "generic")
		return new ViProgramGeneric();
	else if (type == "text")
		return new ViProgramText();

	return nullptr;
}
