#include "ViTexture.h"

ViTexture* ViTexture::Load(std::string path, bool aAlpha, GLint aInternalFormat)
{
	int format = aAlpha ? STBI_rgb_alpha : STBI_rgb;
	int width, height, orig_format;
	uint8_t* data = stbi_load(path.c_str(), &width, &height, &orig_format, format);

	if (data == NULL)
	{
		SDL_Log("Error: Failed to load texture. Reason: %s", stbi_failure_reason());
		return NULL;
	}

	GLenum texFormat = aAlpha ? GL_RGBA : GL_RGB;

	ViTexture* texture = new ViTexture(data, width, height, aInternalFormat, texFormat);
	//stbi_image_free(data);

	return texture;
}

ViTexture::ViTexture(uint8_t* aData, GLsizei aWidth, GLsizei aHeight, GLint aInternalFormat, GLenum aFormat, GLenum aType, GLint aPack, GLint aUnpack, GLenum aMipMap) :
	mWidth(aWidth),
	mHeight(aHeight),
	mInternalFormat(aInternalFormat),
	mFormat(aFormat),
	mType(aType),
	mPack(aPack),
	mUnpack(aUnpack),
	mMipMap(aMipMap),
	mAlpha(false)
{
	if (aInternalFormat == GL_RGBA)
		mAlpha = true;

	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);

	if (aPack > 0)
		glPixelStorei(GL_PACK_ALIGNMENT, aPack);
	if (aUnpack > 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, aUnpack);

	glTexImage2D(GL_TEXTURE_2D, 0, aInternalFormat, mWidth, mHeight, 0, aFormat, aType, aData);

	if (aMipMap)
	{
		glHint(GL_GENERATE_MIPMAP_HINT, aMipMap);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	mData = aData;
}

ViTexture::ViTexture(GLsizei aWidth, GLsizei aHeight, GLint aInternalFormat, GLenum aFormat, GLenum aType, GLint aPack, GLint aUnpack, GLenum aMipMap) :
	mWidth(aWidth),
	mHeight(aHeight),
	mInternalFormat(aInternalFormat),
	mFormat(aFormat),
	mType(aType),
	mPack(aPack),
	mUnpack(aUnpack),
	mMipMap(aMipMap),
	mAlpha(false)
{
	if (aInternalFormat == GL_RGBA)
		mAlpha = true;

	glGenTextures(1, &mId);
	glBindTexture(GL_TEXTURE_2D, mId);

	if (aPack > 0)
		glPixelStorei(GL_PACK_ALIGNMENT, aPack);
	if (aUnpack > 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, aUnpack);

	int channels = GetChannels(aFormat);
	int size = aWidth * aHeight * channels * sizeof(uint8_t);
	uint8_t* data = (uint8_t*)malloc(size);
	glTexImage2D(GL_TEXTURE_2D, 0, aInternalFormat, mWidth, mHeight, 0, aFormat, aType, data);

	if (aMipMap)
	{
		glHint(GL_GENERATE_MIPMAP_HINT, aMipMap);
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	mData = data;
}

ViTexture::~ViTexture()
{
	//TODO test this - it might bug out since we're not using stbi_free?
	free(mData);
}

void ViTexture::UpdateTextureFromGpu()
{
	glBindTexture(GL_TEXTURE_2D, mId);
	glGetTexImage(GL_TEXTURE_2D, 0, mFormat, mType, mData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ViTexture::WritePNG(const char * aFileName)
{
	UpdateTextureFromGpu();

	stbi_flip_vertically_on_write(true);
	int out = stbi_write_png(aFileName, mWidth, mHeight, GetChannels(mInternalFormat), mData, mWidth * GetChannels(mInternalFormat));
}

vec4i ViTexture::GetPixel(vec2i aPosition)
{
	if (mAlpha)
	{
		const uint8_t *p = mData + (4 * (aPosition.y * mWidth + aPosition.x));
		uint8_t r = p[0];
		uint8_t g = p[1];
		uint8_t b = p[2];
		uint8_t a = p[3];

		return vec4i(r, g, b, a);
	}
	else 
	{
		const uint8_t *p = mData + (3 * (aPosition.y * mWidth + aPosition.x));
		uint8_t r = p[0];
		uint8_t g = p[1];
		uint8_t b = p[2];

		return vec4i(r, g, b, 1.0);
	}
}

uint8_t ViTexture::GetChannels(GLint aInternalFormat)
{
	switch (aInternalFormat)
	{
	case (GL_DEPTH_COMPONENT):
		return 1;
	case (GL_DEPTH_STENCIL):
		return 2;
	case (GL_RED):
		return 1;
	case (GL_RG):
		return 2;
	case (GL_RGB):
		return 3;
	case (GL_RGBA):
		return 4;
	default:
		return 0;
	}
}
