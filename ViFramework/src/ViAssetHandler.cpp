#include "ViAssetHandler.h"

ViAssetHandler::ViAssetHandler() :
	mTextureHolder(new ViAssetHolderTexture())
{
}

ViTexture* ViAssetHandler::LoadTexture(std::string aName)
{
	return mTextureHolder->GetAsset(aName);
}

ViMaterial* ViAssetHandler::LoadMaterial(std::string aName)
{
	return mMaterialHolder->GetAsset(aName);
}

ViProgram * ViAssetHandler::LoadProgram(std::string aName)
{
	return mProgramHolder->GetAsset(aName);
}

void ViAssetHandler::InitialParse(std::string assetsvifpath)
{
	ViVifParser parser(assetsvifpath);

	vi_assert(parser.GetValid(), "Error: Assets vif either does not exist or is not parsable.");

	for (ViVifLine line : parser.GetLines())
	{
		if (line.mWords[0] == "textures")
			mTextureHolder->Find("./Assets/" + line.mWords[1]);
	}
}
