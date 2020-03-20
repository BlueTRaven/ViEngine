#pragma once

#include <GL/glew.h>

class ViShader { enum ShaderType; };

struct ViCompiledShader
{
	GLuint mId;
	bool compiled;
	ViShader::ShaderType shaderType;
};