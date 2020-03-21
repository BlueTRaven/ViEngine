#include "ViShader.h"

ViShader* ViShader::Load(ViCompiledShader::ViShaderType type, std::string path)
{
	printf("Loading shader from %s\n", path.c_str());

	std::ifstream file(path);

	if (file.bad())
		return nullptr;

	std::stringstream strstream;
	strstream << file.rdbuf();
	std::string full = strstream.str();

	return new ViShader(type, full);
}

ViShader::ViShader(ViCompiledShader::ViShaderType aShaderType, std::string aFullShader) :
	mShaderType(aShaderType),
	mFullShader(aFullShader)
{
}

ViCompiledShader ViShader::CompileShader()
{
	const char* full_c_str = mFullShader.c_str();

	GLint shader = glCreateShader(mShaderType);
	glShaderSource(shader, 1, &full_c_str, NULL);
	glCompileShader(shader);

	GLint length;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
	if (length > 0)
	{
		GLchar * log = new GLchar[length];
		glGetShaderInfoLog(shader, length, NULL, log);

		printf("%s\0\n", log);

		glDeleteShader(shader);
		shader = -1;
		delete[] log;

		return ViCompiledShader();
	}
	else
	{
		ViCompiledShader compiled = ViCompiledShader(shader, true, mShaderType);
		return compiled;
	}
	return ViCompiledShader();
}
