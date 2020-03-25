#include "ViAssetHolderTexture.h"

ViTexture* ViAssetHolderTexture::LoadAsset(ViAssetDefinition aDefinition)
{
	//This should really never get hit
	vi_assert((aDefinition.type == "texture"), "Error: asset definition has incorrect type. Expected 'texture' but got " + aDefinition.type + ".");

	std::string location = FindLine(aDefinition, "location").mWords[1];

	bool alpha = true;
	GLuint format = GL_RGBA;
	ViVifLine formatLine = FindLine(aDefinition, "format");

	if (!formatLine.mIsEmpty)
	{
		std::string formatStr = formatLine.mWords[1];

		//TODO support more formatting options
		if (formatStr == "RGB")
		{
			alpha = false;
			format = GL_RGB;
		}
		else if (formatStr == "RGBA")
		{
			alpha = true;
			format = GL_RGBA;
		}
	}

	return ViTexture::Load(GetBasePath() + "/" + location, alpha, format);
}
