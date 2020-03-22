#include "ViAssetHolderFont.h"

ViFont* ViAssetHolderFont::LoadAsset(ViAssetDefinition aDefinition)
{
	//This should really never get hit
	vi_assert((aDefinition.type == "font"), "Error: asset definition has incorrect type. Expected 'font' but got " + aDefinition.type + ".");

	std::string size = FindLine(aDefinition, "size").mWords[1];
	std::string location = FindLine(aDefinition, "location").mWords[1];

	return ViFont::Load(std::stof(size), GetBasePath() + "/" + location);
}
