#pragma once

#include <GL/glew.h>

#include "ViUtil.h"

struct VIENGINE_EXPORT ViVertexBatchSettings
{
	enum CullMode
	{
		cCULL_NONE,
		cCULL_CW,
		cCULL_CCW
	};

	enum DepthMode
	{
		cDEPTH_NONE,
		cDEPTH_LESS,
		cDEPTH_GREATER
	};

	enum TexMode
	{
		cWRAP_POINT,	//wrap
		cWRAPM_POINT,	//wrap mirrored
		cCLAMP_POINT,	//clamp
		cWRAP_LINEAR,
		cWRAPM_LINEAR,
		cCLAMP_LINEAR
	};

	enum BlendMode
	{
		cBLEND_NONE,
		cBLEND_ADDITIVE,
		cBLEND_ALPHABLEND,
		cBLEND_NONPREMULTIPLIED,
		cBLEND_OPAQUE,
		cBLEND_CUSTOM
	};

	CullMode cullMode;
	DepthMode depthMode;
	TexMode textureMode;
	BlendMode blendMode;

	GLenum blendModesFactor;
	GLenum blendModedFactor;

	ViVertexBatchSettings(CullMode aCullMode, DepthMode aDepthMode, TexMode aTextureMode, BlendMode aBlendMode);

	static const ViVertexBatchSettings Default;

	void SetSettings();

	void SetTextureSettings();
};