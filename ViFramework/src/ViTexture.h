#pragma once

#include <GL/glew.h>
#include <SDL.h>
#include <string>

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

#include "ViUtil.h"

class VIENGINE_EXPORT ViTexture
{
public:
	static ViTexture* Load(std::string path, bool aAlpha, GLint aInternalFormat);

	ViTexture(uint8_t* aData, GLsizei aWidth, GLsizei aHeight, GLint aInternalFormat = GL_RGBA, GLenum aFormat = GL_RGBA, GLenum aType = GL_UNSIGNED_BYTE, GLint aPack = -1, GLint aUnpack = -1, GLenum aMipMap = GL_NONE);
	//Data is not provided with this constructor. Instead it is initialized as all 0 based off of the format.
	ViTexture(GLsizei aWidth, GLsizei aHeight, GLint aInternalFormat, GLenum aFormat, GLenum aType = GL_UNSIGNED_BYTE, GLint aPack = -1, GLint aUnpack = -1, GLenum aMipMap = GL_NONE);

	~ViTexture();

	//Gets the texture from the gpu.
	//This is not necessary for using GetPixel in most cases, as texture data is stored on creation and rarely changes.
	void UpdateTextureFromGpu();

	void WritePNG(const char* aFileName);

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
	GLenum mType;
	GLint mPack;
	GLint mUnpack;
	GLenum mMipMap;

	uint8_t* mData;	

	uint8_t GetChannels(GLint aInternalFormat);
};