#pragma once

#include <string>

#include "ViUtil.h"

class ViAsset
{
	vi_property_named(std::string, mAssetName, AssetName);
	vi_property_named(std::string, mAssetType, AssetType);
};