#pragma once

#include "ViInputManager.h"
#include "ViAssetHandler.h"
#include "ViVertexBatch.h"
#include "ViUtil.h"

#define ENVIRONMENT ViEnvironment::GetInstance()

class VIENGINE_EXPORT ViEnvironment
{
public:
	ViEnvironment(int aScreenWidth, int aScreenHeight);

	static ViEnvironment* GetInstance();

	void SetWindow(SDL_Window* aWindow);

	vi_property_get_named(ViInputManager*, mInputManager, InputManager);
	vi_property_get_named(ViAssetHandler*, mAssetHandler, AssetHandler);
	vi_property_get_named(ViVertexBatch*, mVertexBatch, VertexBatch);

	vi_property_get_named(int, mScreenWidth, ScreenWidth);
	vi_property_get_named(int, mScreenHeight, ScreenHeight);

	vi_property_get_named(SDL_Window*, mWindow, Window);

	vi_property_named(bool, mFocused, Focused);

private:
	static ViEnvironment* mEnvironment;

	int mScreenWidth;
	int mScreenHeight;

	SDL_Window* mWindow;

	ViInputManager* mInputManager;
	ViAssetHandler* mAssetHandler;
	ViVertexBatch* mVertexBatch;
};