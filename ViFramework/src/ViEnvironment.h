#pragma once

#include "ViInputManager.h"
#include "ViAssetHandler.h"
#include "ViVertexBatch.h"
#include "ViUtil.h"

#define viEnv ViEnvironment::GetInstance()

class VIENGINE_EXPORT ViEnvironment
{
public:
	ViEnvironment(int aScreenWidth, int aScreenHeight);

	static ViEnvironment* GetInstance();

	vi_property_get_named(ViInputManager*, mInputManager, InputManager);
	vi_property_get_named(ViAssetHandler*, mAssetHandler, AssetHandler);
	vi_property_get_named(ViVertexBatch*, mVertexBatch, VertexBatch);

	vi_property_get_named(int, mScreenWidth, ScreenWidth);
	vi_property_get_named(int, mScreenHeight, ScreenHeight);

private:
	static ViEnvironment* mEnvironment;

	int mScreenWidth;
	int mScreenHeight;

	ViInputManager* mInputManager;
	ViAssetHandler* mAssetHandler;
	ViVertexBatch* mVertexBatch;
};