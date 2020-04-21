#include "ViVertexBatchSettings.h"

const ViVertexBatchSettings ViVertexBatchSettings::Default =
ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_NONE, ViVertexBatchSettings::cCLAMP_LINEAR, ViVertexBatchSettings::cBLEND_ALPHABLEND, ViVertexBatchSettings::cDRAW_FILLED);

ViVertexBatchSettings::ViVertexBatchSettings(CullMode aCullMode, DepthMode aDepthMode, TexMode aTextureMode, BlendMode aBlendMode, DrawMode aDrawMode) :
	cullMode(aCullMode),
	depthMode(aDepthMode),
	textureMode(aTextureMode),
	blendMode(aBlendMode),
	drawMode(aDrawMode)
{
}

void ViVertexBatchSettings::SetSettings()
{
	if (drawMode == ViVertexBatchSettings::cDRAW_FILLED)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	else if (drawMode == ViVertexBatchSettings::cDRAW_LINES)
	{
		//glLineWidth(lineWidth);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

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

void ViVertexBatchSettings::SetTextureSettings()
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

