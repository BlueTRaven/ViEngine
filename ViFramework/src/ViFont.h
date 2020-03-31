#pragma once

#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>
#include <GL/glew.h>

#include "ViUtil.h"
#include "ViTexture.h"

#include "stb/stb_truetype.h"
#include "stb/stb_image_write.h"

class ViMesh;
class ViMaterial;

class VIENGINE_EXPORT ViFont
{
public:
	const static int cFIRSTCHAR = ' ';
	const static int cLASTCHAR = '~';
	const static int cCHARCOUNT = cLASTCHAR - cFIRSTCHAR;
	const static int cATLASWIDTH = 512;
	const static int cATLASHEIGHT = 512;

	static ViFont* Load(float size, std::string path);

	ViFont(uint8_t* aData, float aSize);

	vi_property_get_named(bool, mValid, Valid);
	vi_property_get_named(float, mSize, Size);
	vi_property_get_named(ViTexture*, mTexture, Texture);
	vi_property_get_named(stbtt_packedchar*, mCharInfo, CharInfo);

private:
	bool mValid;
	float mSize;

	ViTexture* mTexture;

	stbtt_packedchar mCharInfo[cCHARCOUNT];
};