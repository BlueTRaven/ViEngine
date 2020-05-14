#include "ViFrameBuffer.h"

ViFrameBuffer::ViFrameBuffer(int aWidth, int aHeight, ColorMode aColor, DepthMode aDepth) :
	mWidth(aWidth),
	mHeight(aHeight),
	mColorMode(aColor),
	mDepthMode(aDepth),
	mTexture(nullptr)
{
	glGenFramebuffers(1, &mId);

	//Need to bind the framebuffer to change some stuff within it.
	glBindFramebuffer(GL_FRAMEBUFFER, mId);

	if (aDepth == DepthMode::cDEPTH)
	{
		glGenRenderbuffers(1, &mDepthBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthBufferId);
	}
	else if (aDepth == DepthMode::cDEPTH_READ)
	{
		mDepthTexture = new ViTexture(nullptr, aWidth, aHeight, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->GetId(), 0);
	}

	if (aColor == cCOLOR_ALL)
	{
		mTexture = new ViTexture(aWidth, aHeight, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, -1, -1, GL_FALSE);

		glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture->GetId(), 0);

		//Not sure I entirely understand what this does...
		//I think it sets what the `out vec4` points to in the fragment shader? So this says that it points to the bound framebuffer.
		GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, drawBuffers);
	}
	else if (aColor == cCOLOR_NONE)
	{
		glDrawBuffer(GL_NONE);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("Error: Framebuffer could not be initialized.");
		delete this;	//dangerous - but should be fine, since we don't do anything after
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ViFrameBuffer::~ViFrameBuffer()
{
	if (mId != 0)
		glDeleteFramebuffers(1, &mId);

	if (mDepthBufferId != 0)
		glDeleteRenderbuffers(1, &mDepthBufferId);

	delete mTexture;
} 
