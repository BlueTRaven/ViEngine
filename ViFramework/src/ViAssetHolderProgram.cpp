#include "ViAssetHolderProgram.h"

ViAssetHolderProgram::ViAssetHolderProgram() : 
	ViAssetHolder<ViProgram*>()
{
	mInstance = this;
	SetFactory(new ViAssetHolderProgramFactory());
}

ViProgram* ViAssetHolderProgram::LoadAsset(ViAssetDefinition aDefinition)
{	
	//This should really never get hit
	vi_assert((aDefinition.type == "program"), "Error: asset definition has incorrect type. Expected 'program' but got " + aDefinition.type + ".");

	std::string location;

	ViVifLine customtype = FindLine(aDefinition, "customtype");

	if (!customtype.mIsEmpty)
	{
		return mFactory->Create(customtype.mWords[1]);
	}
	else 
	{	//not a custom type; needs shaders and stuff manually
		//TODO is this even necessary? How often are we going to use a program without a custom type?
		//TODO implement
	}

	return nullptr;
}

ViAssetHolderProgram* ViAssetHolderProgram::mInstance = nullptr;

void ViAssetHolderProgram::SetFactory(ViAssetHolderProgramFactory* aProgramFactory)
{
	mInstance->mFactory = aProgramFactory;
}
