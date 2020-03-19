#pragma once

#include <SDL.h>
#include <map>
#include <string>

#include "ViTexture.h"

class ViAssetHandler
{
public:
	VIENGINE_EXPORT ViTexture* LoadTexture(std::string path);

private:
	DISABLE_WARNING_PUSH
		DISABLE_WARNING(4251)
		std::map<std::string, ViTexture*> mTextures;
	DISABLE_WARNING_POP
};