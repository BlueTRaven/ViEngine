#include "ViAssetHandler.h"

ViTexture* ViAssetHandler::LoadTexture(std::string path)
{
	if (mTextures.find(path) == mTextures.end())
	{	//it's not there. Load it
		ViTexture* texture = ViTexture::Load(path);

		if (texture != NULL)
		{
			mTextures.emplace(path, texture);
			printf("Loaded texture from %s.\n", path.c_str());
		}

		return texture;
	}

	return mTextures[path];
}
