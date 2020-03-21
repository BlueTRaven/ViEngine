#pragma once

#include "ViProgram.h"
#include "ViTexture.h"
#include "ViVifParser.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViMaterial
{
public:
	ViMaterial(ViProgram* aProgram, ViTexture* aTexture);

	vi_property_named(ViProgram*, mProgram, Program);
	vi_property_named(ViTexture*, mTexture, Texture);
};