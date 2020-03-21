#pragma once

#include <string>
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "ViUtil.h"
#include "ViCompiledShader.h"

class ViShader
{
public:
	static ViShader* Load(ViCompiledShader::ViShaderType type, std::string path);

	ViShader(ViCompiledShader::ViShaderType aShaderType, std::string aFullShader);

	ViCompiledShader CompileShader();

private:
	ViCompiledShader::ViShaderType mShaderType;

	std::string mFullShader;
};