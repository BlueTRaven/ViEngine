#pragma once

#include <map>
#include <unordered_map>

#include "ViFont.h"
#include "ViMaterial.h"
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
	ViMaterialFont(ViFont* aFont, ViMaterial* aMaterial);

	void MakeMesh(char c);

	inline ViMaterialFontCharacterMesh GetCharInfo(char c)
	{
		if (mCharMeshes.find(c) == mCharMeshes.end())
			MakeMesh(c);

		return mCharMeshes[c];
	}

	vec2 MeasureString(string s);

	vi_property_get_named(ViFont*, mFont, Font);
	vi_property_get_named(ViMaterial*, mMaterial, Material);

private:
	ViFont* mFont;
	ViMaterial* mMaterial;

	std::map<char, ViMaterialFontCharacterMesh> mCharMeshes;
};