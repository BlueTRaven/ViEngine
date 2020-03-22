#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <vector>

#include "ViVertexAttribute.h"
#include "ViShader.h"
#include "ViUtil.h"
#include "ViColorsGL.h"

class VIENGINE_EXPORT ViProgram
{
public:
	ViProgram(ViShader* vertShader, ViShader* fragShader, std::vector<ViVertexAttribute*> aVertexAttributes);

	virtual void SetUniforms();

	vi_property_get_named(GLuint, mId, Id);
	vi_property_get_named(std::vector<ViVertexAttribute*>, mVertexAttributes, VertexAttributes);

	vi_property_named(bool, mLinked, Linked);
	vi_property_named(bool, mDirty, Dirty);

	vi_property_get_named(mat4, mObjectMat, ObjectMat);

	virtual void SetObjectMat(mat4 aObjMat);

	void BindAttributes();

protected:

	GLuint GetUniform(std::string name);

	void UniformFloat(float val, std::string name);
	void UniformVec4(vec4 val, std::string name);
	void UniformMat4(mat4 val, bool transpose, std::string name);

	//Compile and link the shaders to the program. If program 
	void CompileAndLink();

private:
	GLuint mId = -1;

	mat4 mObjectMat;
	bool mHasObjectMat = true;
	bool mBoundAttributes;

	DISABLE_WARNING_PUSH
	DISABLE_WARNING(4251)
	GLuint mCachedUniform;
	std::string mCachedUniformName;

	ViShader* mVertShader;
	ViShader* mFragShader;

	std::vector<ViVertexAttribute*> mVertexAttributes;
	DISABLE_WARNING_POP
};