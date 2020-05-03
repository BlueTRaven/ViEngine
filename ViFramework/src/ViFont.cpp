#include "ViFont.h"

#include "ViMesh.h"
#include "ViGlyphInfo.h"

ViFont* ViFont::Load(float size, std::string path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if (!file.is_open())
		return nullptr;

	size_t fileSize = file.tellg();
	
	file.seekg(0, std::ios::beg);
	uint8_t* data = (uint8_t*)malloc(fileSize);
	file.read((char*)data, fileSize);

	ViFont* font = new ViFont(data, size);
	file.close();

	return font;
}

ViFont::ViFont(uint8_t* aData, float aSize) :
	mSize(aSize)
{
	uint8_t* atlasData = (uint8_t*)malloc(cATLASWIDTH * cATLASHEIGHT);

	stbtt_pack_context context;
	if (!stbtt_PackBegin(&context, atlasData, ViFont::cATLASWIDTH, ViFont::cATLASHEIGHT, 0, 1, nullptr))
	{
		printf("Error: Font Failed.\n");
		return;
	}

	stbtt_PackSetOversampling(&context, 2, 2);
	if (!stbtt_PackFontRange(&context, aData, 0, aSize, ViFont::cFIRSTCHAR, ViFont::cCHARCOUNT, mCharInfo))
	{
		printf("Error: Font Packing Failed.\n");
		return;
	}

	stbtt_PackEnd(&context);

	mTexture = new ViTexture(atlasData, cATLASWIDTH, cATLASHEIGHT, GL_RGB, GL_RED, GL_UNSIGNED_BYTE, 1, -1, GL_NICEST);

	mValid = true;
}
