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
		mDepthTexture = new ViTexture(nullptr, aWidth, aHeight, GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT, GL_FLOAT, -1, -1, GL_FALSE);
		glBindTexture(GL_TEXTURE_2D, mDepthTexture->GetId());
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture->GetId(), 0);
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
		glReadBuffer(GL_NONE);
	}

	GLenum frameBufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (frameBufferStatus != GL_FRAMEBUFFER_COMPLETE)
	{
		string errorType = "";
		switch (frameBufferStatus)
		{
		case GL_FRAMEBUFFER_UNDEFINED:
			errorType = "GL_FRAMEBUFFER_UNDEFINED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
			errorType = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
			errorType = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
			errorType = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
			errorType = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
			break;
		case GL_FRAMEBUFFER_UNSUPPORTED:
			errorType = "GL_FRAMEBUFFER_UNSUPPORTED";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
			errorType = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
			break;
		case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
			errorType = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
			break;
		default:
			errorType = "Unknown";
			break;
		}

		printf("Error: Framebuffer could not be initialized. Error: %s\n", errorType.c_str());
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
