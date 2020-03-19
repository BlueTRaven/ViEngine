#include "ViGlyphInfo.h"

ViGlyphInfo::ViGlyphInfo(ViFont* font, uint32_t character, float offX, float offY) :
	offX(offX),
	offY(offY)
{
	stbtt_aligned_quad quad;
	stbtt_GetPackedQuad(font->GetCharInfo(), ViFont::cATLASWIDTH, ViFont::cATLASHEIGHT, character - ViFont::cFIRSTCHAR, &this->offX, &this->offY, &quad, 1);

	float xmin = quad.x0;
	float xmax = quad.x1;
	float ymin = -quad.y1;
	float ymax = -quad.y0;

	positions[0] = { xmin, ymin, 0 };
	positions[1] = { xmin, ymax, 0 };
	positions[2] = { xmax, ymax, 0 };
	positions[3] = { xmax, ymin, 0 };

	uvs[0] = { quad.s0, quad.t1 };
	uvs[1] = { quad.s0, quad.t0 };
	uvs[2] = { quad.s1, quad.t0 };
	uvs[3] = { quad.s1, quad.t1 };
}
