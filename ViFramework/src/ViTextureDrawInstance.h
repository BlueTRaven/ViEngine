#pragma once

#include <GL/glew.h>

#include "ViTexture.h"
#include "ViUtil.h"

class VIENGINE_EXPORT ViTextureDrawInstance
{
public:
	ViTextureDrawInstance(ViTexture* aTexture, int aBinding);
	~ViTextureDrawInstance();

	void Bind();

	vi_property_get_named(ViTexture*, mTexture, Texture);
	vi_property_get_named(int, mBinding, Binding);

private:
	ViTexture* mTexture;
	int mBinding;
};