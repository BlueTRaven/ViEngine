#include "ViAssetHolderProgram.h"

ViProgram* ViAssetHolderProgram::LoadAsset(ViAssetDefinition aDefinition)
{	
	//This should really never get hit
	vi_assert((aDefinition.type == "program"), "Error: asset definition has incorrect type. Expected 'program' but got " + aDefinition.type + ".");

	std::string location;

	ViVifLine customtype = FindLine(aDefinition, "customtype");

	if (!customtype.mIsEmpty)
	{
		if (customtype.mWords[1] == "generic")
			return new ViProgramGeneric();
		else if (customtype.mWords[1] == "text")
			return new ViProgramText();
	}
	else 
	{	//not a custom type; needs shaders and stuff manually
		//TODO is this even necessary? How often are we going to use a program without a custom type?
		//TODO implement
	}

	return nullptr;
}
