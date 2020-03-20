#pragma once

#include <map>
#include <string>

#include "ViAssetHolder.h"
#include "ViProgram.h"
#include "ProgramGeneric.h"
#include "ProgramText.h"

class ViAssetHolderProgram : public ViAssetHolder<ViProgram*>
{
	//TODO this needs a way of adding new program customtypes without hardcoding them. 
	virtual ViProgram* LoadAsset(ViAssetDefinition aDefinition) override;
};