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

	void SetSettings()
	{
		if (cullMode != ViVertexBatchSettings::cCULL_NONE)
		{
			glEnable(GL_CULL_FACE);
			glFrontFace(cullMode == ViVertexBatchSettings::cCULL_CW ? GL_CW : GL_CCW);
			glCullFace(GL_BACK);
		}
		else glDisable(GL_CULL_FACE);

		if (depthMode != ViVertexBatchSettings::cDEPTH_NONE)
		{
			glEnable(GL_DEPTH_TEST);
			glDepthMask(GL_TRUE);
			glDepthFunc(depthMode == ViVertexBatchSettings::cDEPTH_LESS ? GL_LESS : GL_GREATER);
		}
		else glDisable(GL_DEPTH_TEST);

		if (blendMode != ViVertexBatchSettings::cBLEND_NONE)
		{
			glEnable(GL_BLEND);

			switch (blendMode)
			{
			case ViVertexBatchSettings::cBLEND_ADDITIVE:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE);
				break;
			case ViVertexBatchSettings::cBLEND_NONPREMULTIPLIED:
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case ViVertexBatchSettings::cBLEND_ALPHABLEND:
				glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
				break;
			case ViVertexBatchSettings::cBLEND_OPAQUE:
				glBlendFunc(GL_ONE, GL_ZERO);
			case ViVertexBatchSettings::cBLEND_CUSTOM:
				glBlendFunc(blendModesFactor, blendModedFactor);
				break;
			default:
				glDisable(GL_BLEND);
				break;
			}
		}
		else glDisable(GL_BLEND);
	}

	void SetTextureSettings()
	{
		switch (textureMode)
		{
		case ViVertexBatchSettings::TexMode::cWRAP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case ViVertexBatchSettings::TexMode::cWRAPM_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case ViVertexBatchSettings::TexMode::cCLAMP_LINEAR:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			break;
		case ViVertexBatchSettings::TexMode::cWRAP_POINT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case ViVertexBatchSettings::TexMode::cWRAPM_POINT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		case ViVertexBatchSettings::TexMode::cCLAMP_POINT:
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			break;
		}
	}
};