#pragma once

#include "ViAssetHolder.h"
#include "ViMaterial.h"

class ViAssetHolderMaterial : public ViAssetHolder<ViMaterial*>
{
	virtual ViMaterial* LoadAsset(ViAssetDefinition aDefinition) override;
};