#include "ViAssetHolderTexture.h"

ViTexture* ViAssetHolderTexture::LoadAsset(ViAssetDefinition aDefinition)
{
	//This should really never get hit
	vi_assert((aDefinition.type == "texture"), "Error: asset definition has incorrect type. Expected 'texture' but got " + aDefinition.type + ".");

	std::string location;
	for (ViVifLine line : aDefinition.definition)
	{
		if (line.mWords[0] == "location")
			location = line.mWords[1];
	}

	return ViTexture::Load(GetBasePath() + "/" + location);
}
