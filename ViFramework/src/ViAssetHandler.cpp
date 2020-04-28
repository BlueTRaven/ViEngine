#include "ViAssetHandler.h"

ViAssetHandler::ViAssetHandler() :
	mTextureHolder(new ViAssetHolderTexture()),
	mFontHolder(new ViAssetHolderFont()),
	mProgramHolder(new ViAssetHolderProgram()),
	mShaderHolder(new ViAssetHolderShader())
{
}

ViTexture* ViAssetHandler::LoadTexture(std::string aName)
{
	return mTextureHolder->GetAsset(aName);
}

ViFont * ViAssetHandler::LoadFont(std::string aName)
{
	return mFontHolder->GetAsset(aName);
}

ViProgram * ViAssetHandler::LoadProgram(std::string aName)
{
	return mProgramHolder->GetAsset(aName);
}

ViShader* ViAssetHandler::LoadShader(std::string aName)
{
	return mShaderHolder->GetAsset(aName);
}

void ViAssetHandler::InitialParse(std::string assetsvifpath)
{
	ViVifParser parser(assetsvifpath);

	vi_assert(parser.GetValid(), "Error: Assets vif either does not exist or is not parsable.");

	for (ViVifLine line : parser.GetLines())
	{
		std::string type = line.mWords[0];
		std::string location = "./Assets/" + line.mWords[1];

		if (type == "textures")
			mTextureHolder->Find(location);
		else if (type == "fonts")
			mFontHolder->Find(location);
		else if (type == "programs")
			mProgramHolder->Find(location);
		else if (type == "shaders")
			mShaderHolder->Find(location);
	}
}
