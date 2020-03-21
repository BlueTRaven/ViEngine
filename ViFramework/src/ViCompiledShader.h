#pragma once

#include <GL/glew.h>

struct ViCompiledShader
{
	enum ViShaderType
	{
		UNCOMPILED = -1,
		VERTEX = GL_VERTEX_SHADER,
		FRAGMENT = GL_FRAGMENT_SHADER,
		COMPUTE = GL_COMPUTE_SHADER,
		GEOMETRY = GL_GEOMETRY_SHADER
	};

	ViCompiledShader();
	ViCompiledShader(GLuint aId, bool aCompiled, ViShaderType aShaderType);

	GLuint mId;
	bool compiled;
	ViShaderType shaderType;
};