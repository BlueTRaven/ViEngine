#pragma once

#include "ViProgram.h"
#include "ViTexture.h"
#include "ViVifParser.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViMaterial
{
public:
	static ViMaterial* Load(std::string path);

	ViMaterial(ViProgram* aProgram, ViTexture* aTexture);

	vi_property_named(ViProgram*, mProgram, Program);
	vi_property_named(ViTexture*, mTexture, Texture);
};