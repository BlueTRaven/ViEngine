#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <string>

#include "stb/stb_image.h"

#include "ViUtil.h"

class VIENGINE_EXPORT ViTexture
{
public:
	static ViTexture* Load(std::string path, bool aAlpha, GLint aInternalFormat);

	ViTexture(uint8_t* aData, GLsizei aWidth, GLsizei aHeight, GLint aInternalFormat = GL_RGBA, GLenum aFormat = GL_RGBA, GLint aPack = -1, GLint aUnpack = -1, GLenum aMipMap = GL_NONE);

	vi_property_get_named(GLuint, mId, Id);

	vi_property_get_named(bool, mAlpha, Alpha);
	vi_property_get_named(GLenum, mFormat, Format);

private:
	bool mAlpha;

	GLuint mId;
	GLsizei mWidth;
	GLsizei mHeight;
	GLint mInternalFormat;
	GLenum mFormat;
	GLint mPack;
	GLint mUnpack;
	GLenum mMipMap;
};