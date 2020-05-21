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
	mAlpha(false),
	mLoadedFrom("")
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

	glDeleteTextures(1, &mId);
}

void ViTexture::UpdateTextureFromGpu()
{
	glBindTexture(GL_TEXTURE_2D, mId);

	if (mData == nullptr)
	{
		int formatBPP = GetFormatSize(mInternalFormat);

		if (formatBPP == 0)
		{
			printf("Error: Could not update texture from gpu. Its internal format was invalid.\n");
			return;
		}

		//We didn't allocate space for this memory yet, so go ahead and do that
		int size = sizeof(uint8_t) * formatBPP * mWidth * mHeight;
		mData = (uint8_t*)malloc(size);
	}

	glGetTexImage(GL_TEXTURE_2D, 0, mFormat, mType, mData);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void ViTexture::WritePNG(const char * aFileName)
{
	UpdateTextureFromGpu();

	int channels = GetChannels(mInternalFormat);
	int size = GetFormatSize(mInternalFormat);

	if (size == 0)
	{
		printf("Error: Could not write png to %s. Its internal format was invalid.\n", aFileName);
		return;
	}

	stbi_flip_vertically_on_write(true);
	int out = stbi_write_png(aFileName, mWidth, mHeight, 4, mData, mWidth * size);
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
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_COMPONENT16:
	case GL_DEPTH_COMPONENT24:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
		return 1;
	case GL_DEPTH_STENCIL:
	case GL_DEPTH24_STENCIL8:
	case GL_DEPTH32F_STENCIL8:
	case GL_DEPTH32F_STENCIL8_NV:
		return 2;
	case GL_RED:
	case GL_R8:
	case GL_R8I:
	case GL_R8UI:
	case GL_R8_SNORM:
	case GL_R16:
	case GL_R16F:
	case GL_R16I:
	case GL_R16UI:
	case GL_R16_SNORM:
	case GL_R32F:
	case GL_R32I:
	case GL_R32UI:
		return 1;
	case GL_RG:
	case GL_RG8:
	case GL_RG8I:
	case GL_RG8UI:
	case GL_RG8_SNORM:
	case GL_RG16:
	case GL_RG16I:
	case GL_RG16F:
	case GL_RG16UI:
	case GL_RG16_SNORM:
	case GL_RG32I:
	case GL_RG32F:
	case GL_RG32UI:
		return 2;
	case GL_RGB:
	case GL_RGB8:
	case GL_RGB8I:
	case GL_RGB8UI:
	case GL_RGB8_SNORM:
	case GL_RGB16:
	case GL_RGB16I:
	case GL_RGB16F:
	case GL_RGB16UI:
	case GL_RGB16_SNORM:
	case GL_RGB32I:
	case GL_RGB32F:
	case GL_RGB32UI:
		return 3;
	case GL_RGBA:
	case GL_RGBA8:
	case GL_RGBA8I:
	case GL_RGBA8UI:
	case GL_RGBA8_SNORM:
	case GL_RGBA16:
	case GL_RGBA16I:
	case GL_RGBA16F:
	case GL_RGBA16UI:
	case GL_RGBA16_SNORM:
	case GL_RGBA32I:
	case GL_RGBA32F:
	case GL_RGBA32UI:
		return 4;
	default:
		return 0;
	}
}

size_t ViTexture::GetFormatSize(GLenum aInternalFormat)
{
	switch (aInternalFormat)
	{
		//One component - R
	case GL_R8:
	case GL_R8I:
	case GL_R8UI:
	case GL_R8_SNORM:
		return 1;
	case GL_RED:
	case GL_R16:
	case GL_R16F:
	case GL_R16I:
	case GL_R16UI:
	case GL_R16_SNORM:
		return 2;
	case GL_R32F:
	case GL_R32I:
	case GL_R32UI:
		return 4;

		//Two components - RG
	case GL_RG8:
	case GL_RG8I:
	case GL_RG8UI:
	case GL_RG8_SNORM:
		return 2 * 1;
	case GL_RG:
	case GL_RG16:
	case GL_RG16I:
	case GL_RG16F:
	case GL_RG16UI:
	case GL_RG16_SNORM:
		return 2 * 2;
	case GL_RG32I:
	case GL_RG32F:
	case GL_RG32UI:
		return 2 * 4;

		//Three components - RGB
	case GL_RGB8:
	case GL_RGB8I:
	case GL_RGB8UI:
	case GL_RGB8_SNORM:
		return 2 * 1;
	case GL_RGB:
	case GL_RGB16:
	case GL_RGB16I:
	case GL_RGB16F:
	case GL_RGB16UI:
	case GL_RGB16_SNORM:
		return 2 * 2;
	case GL_RGB32I:
	case GL_RGB32F:
	case GL_RGB32UI:
		return 2 * 4;

		//Four components - RGBA
	case GL_RGBA8:
	case GL_RGBA8I:
	case GL_RGBA8UI:
	case GL_RGBA8_SNORM:
		return 2 * 1;
	case GL_RGBA:
	case GL_RGBA16:
	case GL_RGBA16I:
	case GL_RGBA16F:
	case GL_RGBA16UI:
	case GL_RGBA16_SNORM:
		return 2 * 2;
	case GL_RGBA32I:
	case GL_RGBA32F:
	case GL_RGBA32UI:
		return 2 * 4;

	case GL_DEPTH_COMPONENT16:
		return 2;
	case GL_DEPTH_COMPONENT24:
		return 3;
	case GL_DEPTH_COMPONENT:
	case GL_DEPTH_COMPONENT32:
	case GL_DEPTH_COMPONENT32F:
	case GL_DEPTH_STENCIL:
	case GL_DEPTH24_STENCIL8:
		return 4;

	case GL_DEPTH32F_STENCIL8:
	case GL_DEPTH32F_STENCIL8_NV:
		return 5;
	default:
		return 0;	//error case. Handle 0
	}
}
