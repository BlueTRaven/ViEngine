#pragma once

#include "ViAssetHolder.h"
#include "ViTexture.h"

class ViAssetHolderTexture : public ViAssetHolder<ViTexture*>
{
	virtual ViTexture* LoadAsset(ViAssetDefinition aDefinition) override;
};