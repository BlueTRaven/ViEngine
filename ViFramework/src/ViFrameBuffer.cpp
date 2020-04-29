#include "ViFrameBuffer.h"

ViFrameBuffer::ViFrameBuffer(int aWidth, int aHeight, bool aDepth)
{
	glGenFramebuffers(1, &mId);

	//Need to bind the framebuffer to change some stuff within it.
	glBindFramebuffer(GL_FRAMEBUFFER, mId);

	mTexture = new ViTexture(aWidth, aHeight, GL_RGBA, GL_RGBA, -1, -1, GL_FALSE);

	if (aDepth)
	{
		glGenRenderbuffers(1, &mDepthRenderBufferId);
		glBindRenderbuffer(GL_RENDERBUFFER, mDepthRenderBufferId);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, aWidth, aHeight);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, mDepthRenderBufferId);
	}

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mTexture->GetId(), 0);
	
	//Not sure I entirely understand what this does...
	//I think it sets what the `out vec4` points to in the fragment shader? So this says that it points to the bound framebuffer.
	GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

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

	if (mDepthRenderBufferId != 0)
		glDeleteRenderbuffers(1, &mDepthRenderBufferId);

	delete mTexture;
} 
