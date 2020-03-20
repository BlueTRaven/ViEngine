#pragma once

#include <vector>
#include <fstream>
#include <string>
#include <GL/glew.h>

#include "ViUtil.h"
#include "ViTexture.h"

#include "stb/stb_truetype.h"
#include "stb/stb_image_write.h"

class VIENGINE_EXPORT ViFont
{
public:
	const static int cFIRSTCHAR = ' ';
	const static int cLASTCHAR = '~';
	const static int cCHARCOUNT = cLASTCHAR - cFIRSTCHAR;
	const static int cATLASWIDTH = 512;
	const static int cATLASHEIGHT = 512;

	static ViFont* Load(std::string path);

	ViFont(uint8_t* aData);

	vi_property_get_named(bool, mValid, Valid);
	vi_property_get_named(ViTexture*, mTexture, Texture);
	vi_property_get_named(stbtt_packedchar*, mCharInfo, CharInfo);

private:
	bool mValid;

	ViTexture* mTexture;

	stbtt_packedchar mCharInfo[cCHARCOUNT];
};