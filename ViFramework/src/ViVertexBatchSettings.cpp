#include "ViVertexBatchSettings.h"

const ViVertexBatchSettings ViVertexBatchSettings::Default =
ViVertexBatchSettings(ViVertexBatchSettings::cCULL_NONE, ViVertexBatchSettings::cDEPTH_NONE, ViVertexBatchSettings::cCLAMP_LINEAR);

ViVertexBatchSettings::ViVertexBatchSettings(CullMode aCullMode, DepthMode aDepthMode, TexMode aTextureMode) :
	cullMode(aCullMode),
	depthMode(aDepthMode),
	textureMode(aTextureMode)
{
}
