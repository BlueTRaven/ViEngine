#pragma once

#include "ViProgram.h"
#include "ViTexture.h"
#include "ViVifParser.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViMaterial
{
public:
	static const int cMAX_TEXTURES;

	ViMaterial(ViProgram* aProgram, ViTexture* aTexture);
	~ViMaterial();

	void AddTexture(ViTexture* aTexture);

	vi_property_named(ViProgram*, mProgram, Program);
	vi_property_named(ViTexture**, mTextures, Textures);

private:
	int mLastTexture;
};