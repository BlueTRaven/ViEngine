#pragma once

#include <string>
#include <GL/glew.h>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "ViUtil.h"

class ViShader
{
public:
	enum ShaderType
	{
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		COMPUTE = GL_COMPUTE_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER
	};

	static ViShader* Load(ShaderType type, std::string path);

	ViShader(ShaderType aShaderType, GLuint aShaderId);

	vi_property_get(GLuint, mShaderId)
		vi_property_get(ShaderType, mShaderType)

private:
	GLuint mShaderId;
	ShaderType mShaderType;
};