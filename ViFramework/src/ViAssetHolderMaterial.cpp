#include "ViAssetHolderMaterial.h"

#include "ViEnvironment.h"

ViMaterial* ViAssetHolderMaterial::LoadAsset(ViAssetDefinition aDefinition)
{
	//This should really never get hit
	vi_assert((aDefinition.type == "material"), "Error: asset definition has incorrect type. Expected 'material' but got " + aDefinition.type + ".");

	ViVifLine programLine = FindLine(aDefinition, "program");
	std::string programName = programLine.mWords[1];

	ViVifLine textureLine = FindLine(aDefinition, "texture");

	ViVifLine fontLine = FindLine(aDefinition, "font");

	if (textureLine.mIsEmpty)
	{
		if (!fontLine.mIsEmpty)
		{
			std::string fontName = fontLine.mWords[1];

			ViProgram* program = ENVIRONMENT->GetAssetHandler()->LoadProgram(programName);
			ViFont* font = ENVIRONMENT->GetAssetHandler()->LoadFont(fontName);

			return new ViMaterial(program, font->GetTexture());
		}
		else 
		{
			printf("Error: No font or texture definition could be found for material asset %s.\n", aDefinition.name.c_str());
			return nullptr;
		}
	}
	else
	{
		std::string textureName = textureLine.mWords[1];

		ViProgram* program = ENVIRONMENT->GetAssetHandler()->LoadProgram(programName);
		ViTexture* texture = ENVIRONMENT->GetAssetHandler()->LoadTexture(textureName);

		return new ViMaterial(program, texture);
	}
}
