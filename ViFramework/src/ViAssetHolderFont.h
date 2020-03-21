#pragma once

#include "ViFont.h"
#include "ViAssetHolder.h"

class ViAssetHolderFont : public ViAssetHolder<ViFont*>
{
	virtual ViFont* LoadAsset(ViAssetDefinition aDefinition) override;
};