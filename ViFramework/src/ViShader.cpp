#include "ViShader.h"

ViShader* ViShader::Load(ShaderType type, std::string path)
{
	printf("Loading shader from %s\n", path.c_str());

	std::ifstream file(path);

	if (file.bad())
		return nullptr;

	std::stringstream strstream;
	strstream << file.rdbuf();
	std::string full = strstream.str();
	const char * full_c_str = full.c_str();

	GLint shader = glCreateShader(type);
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

		return nullptr;
	}
	else
	{
		ViShader* compiledShader = new ViShader(type, shader);

		return compiledShader;
	}
}

ViShader::ViShader(ShaderType aShaderType, GLuint aShaderId) :
	mShaderType(aShaderType),
	mShaderId(aShaderId)
{
}
