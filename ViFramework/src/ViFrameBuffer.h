#pragma once

#include <GL/glew.h>

#include "ViUtil.h"
#include "ViTexture.h"

class VIENGINE_EXPORT ViFrameBuffer
{
public:
	enum DepthMode
	{
		cDEPTH_NONE,
		cDEPTH,
		cDEPTH_READ	//depth, but readable
	};

	enum ColorMode
	{
		cCOLOR_NONE,
		cCOLOR_ALL
	};

	ViFrameBuffer(int aWidth, int aHeight, ColorMode aColor, DepthMode aDepth);
	~ViFrameBuffer();

	vi_property_named(ViTexture*, mTexture, Texture);

	vi_property_named(GLuint, mId, Id);
	vi_property_named(GLuint, mDepthBufferId, DepthBufferId);

	vi_property_get_named(int, mWidth, Width);
	vi_property_get_named(int, mHeight, Height);

	vi_property_get_named(ColorMode, mColorMode, ColorMode);
	vi_property_get_named(DepthMode, mDepthMode, DepthMode);

private:
	int mWidth;
	int mHeight;

	ColorMode mColorMode;
	DepthMode mDepthMode;
};