#pragma once

#include "ViAssetHolder.h"
#include "ViShader.h"

class ViAssetHolderShader : public ViAssetHolder<ViShader*>
{
	virtual ViShader* LoadAsset(ViAssetDefinition aDefinition) override;
};