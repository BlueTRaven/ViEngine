#pragma once

#include <SDL.h>
#include <map>
#include <string>
#include <stdio.h>
#include <iostream>

#include "ViTexture.h"
#include "ViFont.h"
#include "ViProgram.h"
#include "ViShader.h"
#include "ViVifParser.h"
#include "ViAssetHolderTexture.h"
#include "ViAssetHolderFont.h"
#include "ViAssetHolderProgram.h"
#include "ViAssetHolderShader.h"

//Shortcut macros so we don't have to type so much
#define ASSET_HANDLER ENVIRONMENT->GetAssetHandler()

#define GET_ASSET_TEXTURE(name) ENVIRONMENT->GetAssetHandler()->LoadTexture(name)
#define GET_ASSET_FONT(name) ENVIRONMENT->GetAssetHandler()->LoadFont(name)
#define GET_ASSET_PROGRAM(name) ENVIRONMENT->GetAssetHandler()->LoadProgram(name)
#define GET_ASSET_SHADER(name) ENVIRONMENT->GetAssetHandler()->LoadShader(name)

class ViAssetHandler
{
public:
	ViAssetHandler();

	VIENGINE_EXPORT ViTexture* LoadTexture(std::string aName);
	VIENGINE_EXPORT ViFont* LoadFont(std::string aName);
	VIENGINE_EXPORT ViProgram* LoadProgram(std::string aName);
	VIENGINE_EXPORT ViShader* LoadShader(std::string aName);

	VIENGINE_EXPORT void InitialParse(std::string assetsvifpath);

private:
	ViAssetHolderTexture* mTextureHolder;
	ViAssetHolderFont* mFontHolder;
	ViAssetHolderProgram* mProgramHolder;
	ViAssetHolderShader* mShaderHolder;
};