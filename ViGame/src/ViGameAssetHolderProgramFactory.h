#pragma once

#include "ViAssetHolderProgramFactory.h"

namespace vigame
{
	class ViGameAssetHolderProgramFactory : public ViAssetHolderProgramFactory
	{
		ViProgram* Create(std::string type) override;
	};
}