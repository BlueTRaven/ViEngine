#include "ViAssetHolderShader.h"

ViShader* ViAssetHolderShader::LoadAsset(ViAssetDefinition aDefinition)
{
	//This should really never get hit
	vi_assert((aDefinition.type == "shader"), "Error: asset definition has incorrect type. Expected 'shader' but got " + aDefinition.type + ".");

	ViVifLine type = FindLine(aDefinition, "type");

	std::string typeName = type.mWords[1];
	ViCompiledShader::ViShaderType shaderType = ViCompiledShader::ViShaderType::UNCOMPILED;

	if (typeName == "vertex")
		shaderType = ViCompiledShader::ViShaderType::VERTEX;
	if (typeName == "fragment")
		shaderType = ViCompiledShader::ViShaderType::FRAGMENT;
	if (typeName == "geometry")
		shaderType = ViCompiledShader::ViShaderType::GEOMETRY;
	if (typeName == "compute")
		shaderType = ViCompiledShader::ViShaderType::COMPUTE;

	std::string location = FindLine(aDefinition, "location").mWords[1];

	return ViShader::Load(shaderType, GetBasePath() + "/" + location);
}
