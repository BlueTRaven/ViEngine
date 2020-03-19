#include "ViFont.h"

ViFont* ViFont::Load(std::string path)
{
	std::ifstream file(path, std::ios::binary | std::ios::ate);

	if (!file.is_open())
		return nullptr;

	size_t size = file.tellg();
	
	file.seekg(0, std::ios::beg);
	uint8_t* data = (uint8_t*)malloc(size);
	file.read((char*)data, size);

	ViFont* font = new ViFont(data);
	file.close();

	return font;
}

ViFont::ViFont(uint8_t* aData) 
{
	uint8_t* atlasData = (uint8_t*)malloc(cATLASWIDTH * cATLASHEIGHT);

	stbtt_pack_context context;
	if (!stbtt_PackBegin(&context, atlasData, ViFont::cATLASWIDTH, ViFont::cATLASHEIGHT, 0, 1, nullptr))
	{
		printf("Error: Font Failed.\n");
		return;
	}

	stbtt_PackSetOversampling(&context, 2, 2);
	if (!stbtt_PackFontRange(&context, aData, 0, 40, ViFont::cFIRSTCHAR, ViFont::cCHARCOUNT, mCharInfo))
	{
		printf("Error: Font Packing Failed.\n");
		return;
	}

	stbtt_PackEnd(&context);

	mTexture = new ViTexture(atlasData, cATLASWIDTH, cATLASHEIGHT, GL_RGB, GL_RED, 1, -1, GL_NICEST);

	mValid = true;
}
