#pragma once

#include <GL/glew.h>

#include "ViUtil.h"
#include "ViTexture.h"

class VIENGINE_EXPORT ViFrameBuffer
{
public:
	ViFrameBuffer(int aWidth, int aHeight, bool aDepth);
	~ViFrameBuffer();

	vi_property_named(ViTexture*, mTexture, Texture);
	vi_property_named(GLuint, mId, Id);
	vi_property_named(GLuint, mDepthRenderBufferId, DepthRenderBufferId);
};