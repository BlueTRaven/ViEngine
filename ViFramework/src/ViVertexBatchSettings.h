#pragma once

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

	CullMode cullMode;
	DepthMode depthMode;
	TexMode textureMode;

	ViVertexBatchSettings(CullMode aCullMode, DepthMode aDepthMode, TexMode aTextureMode);

	static const ViVertexBatchSettings Default;
};