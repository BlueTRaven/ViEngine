#include "ViAssetHolderMaterial.h"

ViMaterial* ViAssetHolderMaterial::LoadAsset(ViAssetDefinition aDefinition)
{
	//This should really never get hit
	vi_assert((aDefinition.type == "material"), "Error: asset definition has incorrect type. Expected 'material' but got " + aDefinition.type + ".");

	std::string location;
	for (ViVifLine line : aDefinition.definition)
	{
		if (line.mWords[0] == "location")
			location = line.mWords[1];
	}
}
