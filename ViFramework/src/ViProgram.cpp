#include "ViProgram.h"

ViProgram::ViProgram(ViShader* vertShader, ViShader* fragShader, std::vector<ViVertexAttribute*> aVertexAttributes) :
	program(glCreateProgram()),
	mVertexAttributes(aVertexAttributes)
{
	glAttachShader(program, vertShader->Get_mShaderId());
	
	glAttachShader(program, fragShader->Get_mShaderId());

	glLinkProgram(program);

	GLint linked = 0;
	glGetProgramiv(program, GL_LINK_STATUS, (int*)&linked);
	if (linked == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			GLchar * log = new GLchar[length];
			glGetProgramInfoLog(program, length, NULL, log);
			printf("%s\0\n", log);

			//note that we don't detach and delete the shader, because that will be done anyways.
			delete[] log;
		}
	}

	glDetachShader(program, vertShader->Get_mShaderId());
	glDeleteShader(vertShader->Get_mShaderId());
	
	glDetachShader(program, fragShader->Get_mShaderId());
	glDeleteShader(fragShader->Get_mShaderId());

	delete vertShader;
	delete fragShader;

	BindAttributes();

	if (linked == GL_TRUE)
		printf("Linked a shader successfully.\n");
}

void ViProgram::SetUniforms()
{
}

void ViProgram::SetObjectMat(mat4 aObjectMat)
{
	SetDirty(true);
	mObjectMat = aObjectMat;

	if (mHasObjectMat)
		UniformMat4(aObjectMat, false, "object");
}

GLuint ViProgram::GetUniform(std::string name)
{
	GLint uniform = glGetUniformLocation(program, name.c_str());
	if (uniform < -1)
	{
		printf("Could not find uniform %s position.\n", name.c_str());

		return -1;
	}
	return uniform;
}

void ViProgram::UniformFloat(float val, std::string name)
{
	GLuint uniform = GetUniform(name);
	if (uniform < 0)
	{
		printf("Float Uniform %s could not be found.\n", name.c_str());
		return;
	}

	glUniform1f(uniform, val);
}

void ViProgram::UniformVec4(vec4 val, std::string name)
{
	GLuint uniform = GetUniform(name);
	if (uniform < 0)
	{
		printf("Vec4 Uniform %s could not be found.\n", name.c_str());
		return;
	}

	glUniform4f(uniform, val.x, val.y, val.z, val.w);
}

void ViProgram::UniformMat4(mat4 val, bool transpose, std::string name)
{
	GLuint uniform = GetUniform(name);
	if (uniform < 0)
	{
		printf("Mat4 Uniform %s could not be found.\n", name.c_str());
		return;
	}

	auto data = glm::value_ptr(val);
	glUniformMatrix4fv(uniform, 1, transpose, data);
}

void ViProgram::BindAttributes()
{
	for (auto attribute : mVertexAttributes)
	{
		if (attribute->Get_elements() > 4)
		{
			printf("Cannot have more than 4 fields. Was given %i.\n", attribute->Get_elements());
			return;
		}

		GLuint attribId = glGetAttribLocation(Get_program(), attribute->Get_name().c_str());

		if (attribId == -1)
		{
			printf("Could not find attribute %s position.\n", attribute->Get_name().c_str());
			return;
		}

		attribute->Set_id(attribId);
	}
}
