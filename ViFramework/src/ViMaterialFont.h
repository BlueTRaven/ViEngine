#pragma once

#include <map>
#include <unordered_map>

#include "ViProgram.h"
#include "ViFont.h"
#include "ViUtil.h"

class ViMesh;
struct ViVertex;

struct VIENGINE_EXPORT ViMaterialFontCharacterMesh
{
	char c;
	ViMesh* mesh;
	float width;
	float height;
	float offX;
	float offY;
};

class VIENGINE_EXPORT ViMaterialFont
{
public:
	ViMaterialFont(ViFont* aFont, ViProgram* aProgram);

	void MakeCharMesh(char c);

	inline ViMaterialFontCharacterMesh GetCharMeshInfo(char c)
	{
		if (mCharMeshes.find(c) == mCharMeshes.end())
			MakeCharMesh(c);

		return mCharMeshes[c];
	}

	vec2 MeasureString(string s);

	vi_property_get_named(ViFont*, mFont, Font);
	vi_property_get_named(ViProgram*, mProgram, Program);

private:
	ViFont* mFont;
	ViProgram* mProgram;

	std::map<char, ViMaterialFontCharacterMesh> mCharMeshes;
};