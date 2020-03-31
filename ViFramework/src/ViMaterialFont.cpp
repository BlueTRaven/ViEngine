#include "ViMaterialFont.h"

#include "ViMesh.h"
#include "ViVertex.h"
#include "ViGlyphInfo.h"

ViMaterialFont::ViMaterialFont(ViFont * aFont, ViMaterial * aMaterial) :
	mFont(aFont),
	mMaterial(aMaterial)
{
}

void ViMaterialFont::MakeMesh(char c)
{
	if (mCharMeshes.find(c) != mCharMeshes.end())
		delete mCharMeshes[c].mesh;	//recreate the mesh

	std::vector<ViVertex> vertices;
	std::vector<GLuint> indices;

	float offX = 0, offY = 0;
	ViGlyphInfo glyph(mFont, c, offX, offY);
	offX = glyph.offX;
	offY = glyph.offY;

	vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[0], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[0]));
	vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[1], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[1]));
	vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[2], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[2]));
	vertices.emplace_back(ViVertex({ 0.0, 0.0, -1.0 }, glyph.positions[3], ViColorGL(1.0, 1.0, 1.0, 1.), glyph.uvs[3]));
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(0);
	indices.push_back(2);
	indices.push_back(3);

	ViMesh* mesh = new ViMesh(mMaterial, vertices, indices);
	mesh->UploadData();

	ViMaterialFontCharacterMesh charMesh;
	charMesh.c = c;
	charMesh.mesh = mesh;
	charMesh.offX = (mFont->GetCharInfo() + (c - ViFont::cFIRSTCHAR))->xadvance;
	charMesh.offY = 0;
	charMesh.width = (glyph.positions[2] - glyph.positions[0]).x;
	charMesh.height = (glyph.positions[1] - glyph.positions[0]).y;

	mCharMeshes.emplace(c, charMesh);
}

vec2 ViMaterialFont::MeasureString(string s)
{
	float x = 0;
	float y = 0;
	for (int i = 0; i < s.size(); i++)
	{
		auto charInfo = GetCharInfo(s[i]);
		x += charInfo.width;

		if (charInfo.height > y)
			y = charInfo.height;
	}

	return vec2(x, y);
}
