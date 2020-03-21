#include "ViCompiledShader.h"

ViCompiledShader::ViCompiledShader() :
	mId(-1),
	compiled(false),
	shaderType(ViShaderType::UNCOMPILED)
{
}

ViCompiledShader::ViCompiledShader(GLuint aId, bool aCompiled, ViShaderType aShaderType) :
	mId(aId),
	compiled(aCompiled),
	shaderType(aShaderType)
{
}
