#include "ViAssetHolderProgram.h"

ViProgram* ViAssetHolderProgram::LoadAsset(ViAssetDefinition aDefinition)
{	
	//This should really never get hit
	vi_assert((aDefinition.type == "program"), "Error: asset definition has incorrect type. Expected 'program' but got " + aDefinition.type + ".");

	std::string location;
	for (ViVifLine line : aDefinition.definition)
	{
		if (line.mWords[0] == "customtype")
		{
			if (line.mWords[1] == "generic")
				return new ViProgramGeneric();
			else if (line.mWords[1] == "text")
				return new ViProgramText();
		}
	}
}
