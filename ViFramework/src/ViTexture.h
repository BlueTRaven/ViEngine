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
	//Data is not provided with this constructor. Instead it is initialized as all 0 based off of the format.
	ViTexture(GLsizei aWidth, GLsizei aHeight, GLint aInternalFormat, GLenum aFormat, GLint aPack = -1, GLint aUnpack = -1, GLenum aMipMap = GL_NONE);

	~ViTexture();

	vec4i GetPixel(vec2i aPosition);

	vi_property_get_named(GLuint, mId, Id);

	vi_property_get_named(bool, mAlpha, Alpha);
	vi_property_get_named(GLenum, mFormat, Format);

	vi_property_get_named(GLsizei, mWidth, Width);
	vi_property_get_named(GLsizei, mHeight, Height);

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

	uint8_t* mData;	

	uint8_t GetStrideFromInternalFormat(GLint aInternalFormat);
};