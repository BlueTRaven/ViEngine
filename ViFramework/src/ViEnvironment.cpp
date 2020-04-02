#include "ViEnvironment.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"
#undef STB_IMAGE_IMPLEMENTATION

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb/stb_truetype.h"
#undef STB_TRUETYPE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"
#undef STB_IMAGE_WRITE_IMPLEMENTATION

ViEnvironment* ViEnvironment::mEnvironment = nullptr;

ViEnvironment::ViEnvironment(int aScreenWidth, int aScreenHeight) :
	mInputManager(new ViInputManager()),
	mAssetHandler(new ViAssetHandler()),
	mVertexBatch(new ViVertexBatch()),
	mWindow(nullptr),
	mScreenWidth(aScreenWidth),
	mScreenHeight(aScreenHeight)
{
	assert(mEnvironment == nullptr);
	mEnvironment = this;
}

void ViEnvironment::SetWindow(SDL_Window* aWindow)
{
	if (mWindow != nullptr)
	{
		printf("Error: cannot set aWindow again.");
		return;
	}
	mWindow = aWindow;
}

ViEnvironment* ViEnvironment::GetInstance()
{
	return mEnvironment;
}
