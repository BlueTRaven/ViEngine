#include "ViMaterial.h"

const int ViMaterial::cMAX_TEXTURES = 16;

ViMaterial::ViMaterial(ViProgram* aProgram, ViTexture* aTexture) :
	mProgram(aProgram),
	mTextures(nullptr)
{
	mTextures = (ViTexture**)malloc(sizeof(ViTexture*) * cMAX_TEXTURES);
	memset(mTextures, 0, sizeof(ViTexture*) * cMAX_TEXTURES);

	mTextures[0] = aTexture;
	mLastTexture = 1;
}

ViMaterial::~ViMaterial()
{
	//We don't want to actually delete any of the textures, just the allocated memory for their pointers.
	//So just set all the pointers to null.
	for (int i = 0; i < cMAX_TEXTURES; i++)
	{
		mTextures[i] = nullptr;
	}

	free(mTextures);
}

void ViMaterial::AddTexture(ViTexture * aTexture)
{
	mTextures[mLastTexture] = aTexture;
	mLastTexture++;
}
