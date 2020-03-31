#pragma once

#include <unordered_map>
#include <string>
#include <functional>

#include "ViAssetHolder.h"
#include "ViProgram.h"
#include "ProgramGeneric.h"
#include "ProgramText.h"
#include "ViAssetHolderProgramFactory.h"

class ViAssetHolderProgram : public ViAssetHolder<ViProgram*>
{
public:
	ViAssetHolderProgram();

	//TODO this needs a way of adding new program customtypes without hardcoding them. 
	virtual ViProgram* LoadAsset(ViAssetDefinition aDefinition) override;

	static VIENGINE_EXPORT void SetFactory(ViAssetHolderProgramFactory* aProgramFactory);

private:
	static ViAssetHolderProgram* mInstance;

	ViAssetHolderProgramFactory* mFactory;
};