#include "ViTextureDrawInstance.h"

ViTextureDrawInstance::ViTextureDrawInstance(ViTexture* aTexture, int aBinding) : 
	mTexture(aTexture),
	mBinding(aBinding)
{
}

ViTextureDrawInstance::~ViTextureDrawInstance()
{
	//ViTextureDrawInstance does NOT own the texture. Therefore, set it to null before we deallocate anything.
	mTexture = nullptr;

	//Pointless?
	delete mTexture;
}

void ViTextureDrawInstance::Bind()
{
	glActiveTexture(GL_TEXTURE0 + mBinding);

	glBindTexture(GL_TEXTURE_2D, mTexture->GetId());
}
