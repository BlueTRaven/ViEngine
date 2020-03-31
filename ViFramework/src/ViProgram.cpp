#include "ViProgram.h"

#include "ViVertexBatchInstance.h"
#include "ViVertexBatchSettings.h"

ViProgram::ViProgram(ViShader* vertShader, ViShader* fragShader, std::vector<ViVertexAttribute> aVertexAttributes) :
	mVertShader(vertShader),
	mFragShader(fragShader),
	mVertexAttributes(aVertexAttributes),
	mBoundAttributes(false),
	mUniformObjectMat(new ViUniformMat4("object", glm::identity<mat4>()))
{
	CompileAndLink();
}

void ViProgram::Bind(ViVertexBatchInstance& aBindTo)//, ViVertexBatchSettings aSettings)
{
	glUseProgram(aBindTo.mesh->GetSubsection(aBindTo.meshSubsection).material->GetProgram()->GetId());

	BindAttributes(aBindTo, false);
}

void ViProgram::SetUniforms(ViVertexBatchInstance& aInstance)
{
	if (!GetDirty())
		return;

	if (mHasObjectMat)
		mUniformObjectMat->Upload(this); //UniformMat4(mObjectMat, false, cOBJECT_UNIFORM);
}

void ViProgram::SetObjectMat(mat4 aObjectMat)
{
	SetDirty(true);
	mUniformObjectMat->Set(this, aObjectMat);
	//mObjectMat = aObjectMat;
}

void ViProgram::BindAttributes(ViVertexBatchInstance& aInstance, bool aForceBind)
{
	//TODO optimize
	for (auto& attribute : mVertexAttributes)
	{
		if (!mBoundAttributes)
		{
			if (attribute.Get_elements() > 4)
			{
				printf("Cannot have more than 4 fields. Was given %i.\n", attribute.Get_elements());
				continue;
			}

			GLuint attribId = glGetAttribLocation(mId, attribute.Get_name().c_str());

			if (attribId == -1)
			{
				printf("Could not find attribute %s position.\n", attribute.Get_name().c_str());
				continue;
			}

			attribute.Set_id(attribId);

			glBindAttribLocation(mId, attribute.Get_id(), attribute.Get_name().c_str());
		}

		glVertexAttribPointer(attribute.Get_id(), attribute.Get_elements(), GL_FLOAT, GL_FALSE, (GLsizei)attribute.Get_size(), attribute.Get_offset());
		glEnableVertexAttribArray(attribute.Get_id());
	}

	mBoundAttributes = true;
}

void ViProgram::BindAttribute(ViVertexAttribute& aAttribute)
{
	if (aAttribute.Get_elements() > 4)
	{
		printf("Cannot have more than 4 fields. Was given %i.\n", aAttribute.Get_elements());
		return;
	}

	GLuint attribId = glGetAttribLocation(mId, aAttribute.Get_name().c_str());

	if (attribId == -1)
	{
		printf("Could not find attribute %s position.\n", aAttribute.Get_name().c_str());
		return;
	}

	aAttribute.Set_id(attribId);

	glEnableVertexAttribArray(aAttribute.Get_id());
	glBindAttribLocation(mId, aAttribute.Get_id(), aAttribute.Get_name().c_str());

	glVertexAttribPointer(aAttribute.Get_id(), aAttribute.Get_elements(), GL_FLOAT, GL_FALSE, (GLsizei)aAttribute.Get_size(), aAttribute.Get_offset());
}

/*
//TODO cache using a custom hashmap
//std::map will definitely not work, too slow
GLuint ViProgram::GetUniform(std::string name)
{
	if (mCachedUniformName == name && mCachedUniform != -1)
		return mCachedUniform;

	if (!mLinked)
	{
		printf("Program has not been linked - uniform %s cannot be found.\n", name.c_str());
		mCachedUniform = -1;
		return -1;
	}

	GLint uniform = glGetUniformLocation(mId, name.c_str());
	if (uniform < -1)
	{
		printf("Could not find uniform %s position.\n", name.c_str());
		mCachedUniform = -1;
		return -1;
	}

	mCachedUniform = uniform;
	mCachedUniformName = name;
	return uniform;
}

void ViProgram::UniformFloat(float val, std::string name)
{
	GLuint uniform = GetUniform(name);
	if (uniform < 0)
		return;

	glUniform1f(uniform, val);
}

void ViProgram::UniformVec4(vec4 val, std::string name)
{
	GLuint uniform = GetUniform(name);
	if (uniform < 0)
		return;

	glUniform4f(uniform, val.x, val.y, val.z, val.w);
}

void ViProgram::UniformVec3(vec3 val, std::string name)
{
	GLuint uniform = GetUniform(name);
	if (uniform < 0)
		return;

	glUniform3f(uniform, val.x, val.y, val.z);
}

void ViProgram::UniformMat4(mat4 val, bool transpose, std::string name)
{
	GLuint uniform = GetUniform(name);
	if (uniform < 0)
		return;

	auto data = glm::value_ptr(val);
	glUniformMatrix4fv(uniform, 1, transpose, data);
}*/

void ViProgram::CompileAndLink()
{
	if (mId != -1)
		glDeleteProgram(mId);
	mId = glCreateProgram();

	ViCompiledShader vertShaderCompiled = mVertShader->CompileShader();
	ViCompiledShader fragShaderCompiled = mFragShader->CompileShader();

	glAttachShader(mId, vertShaderCompiled.mId);

	glAttachShader(mId, fragShaderCompiled.mId);

	glLinkProgram(mId);

	GLint linked = 0;
	glGetProgramiv(mId, GL_LINK_STATUS, (int*)&linked);
	if (linked == GL_FALSE)
	{
		GLint length;
		glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &length);
		if (length > 0)
		{
			GLchar * log = new GLchar[length];
			glGetProgramInfoLog(mId, length, NULL, log);
			printf("%s\0\n", log);

			//note that we don't detach and delete the shader, because that will be done anyways.
			delete[] log;
		}
	}

	glDetachShader(mId, vertShaderCompiled.mId);
	glDeleteShader(vertShaderCompiled.mId);

	glDetachShader(mId, fragShaderCompiled.mId);
	glDeleteShader(fragShaderCompiled.mId);

	//BindAttributes();

	if (linked == GL_TRUE)
		SetLinked(true);
}
