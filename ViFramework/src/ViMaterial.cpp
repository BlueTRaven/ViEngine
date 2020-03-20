#include "ViMaterial.h"

ViMaterial * ViMaterial::Load(std::string path)
{
	/*ViVifParser parser(path);

	for (ViVifLine line : parser.GetLines())
	{
	}*/
	return nullptr;
}

ViMaterial::ViMaterial(ViProgram* aProgram, ViTexture* aTexture) :
	mProgram(aProgram),
	mTexture(aTexture)
{
}
