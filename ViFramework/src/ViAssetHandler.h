#pragma once

#include <SDL.h>
#include <map>
#include <string>
#include <stdio.h>
#include <iostream>

#include "ViTexture.h"
#include "ViFont.h"
#include "ViMaterial.h"
#include "ViProgram.h"
#include "ViShader.h"
#include "ViVifParser.h"
#include "ViAssetHolderTexture.h"
#include "ViAssetHolderFont.h"
#include "ViAssetHolderMaterial.h"
#include "ViAssetHolderProgram.h"
#include "ViAssetHolderShader.h"

//Shortcut macros so we don't have to type so much
#define ASSET_HANDLER viEnv->GetAssetHandler()

#define GET_ASSET_TEXTURE(name) viEnv->GetAssetHandler()->LoadTexture(name)
#define GET_ASSET_FONT(name) viEnv->GetAssetHandler()->LoadFont(name)
#define GET_ASSET_MATERIAL(name) viEnv->GetAssetHandler()->LoadMaterial(name)
#define GET_ASSET_PROGRAM(name) viEnv->GetAssetHandler()->LoadProgram(name)
#define GET_ASSET_SHADER(name) viEnv->GetAssetHandler()->LoadShader(name)

class ViAssetHandler
{
public:
	ViAssetHandler();

	VIENGINE_EXPORT ViTexture* LoadTexture(std::string aName);
	VIENGINE_EXPORT ViFont* LoadFont(std::string aName);
	VIENGINE_EXPORT ViMaterial* LoadMaterial(std::string aName);
	VIENGINE_EXPORT ViProgram* LoadProgram(std::string aName);
	VIENGINE_EXPORT ViShader* LoadShader(std::string aName);

	VIENGINE_EXPORT void InitialParse(std::string assetsvifpath);

private:
	ViAssetHolderTexture* mTextureHolder;
	ViAssetHolderFont* mFontHolder;
	ViAssetHolderMaterial* mMaterialHolder;
	ViAssetHolderProgram* mProgramHolder;
	ViAssetHolderShader* mShaderHolder;
};