#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <unordered_set>
#include <vector>

#include "ViVertexAttribute.h"
#include "ViShader.h"
#include "ViUtil.h"
#include "ViColorsGL.h"
#include "ViUniform.h"
#include "ViUniformMat4.h"

struct ViVertexBatchInstance;
struct ViVertexBatchSettings;
class ViMesh;

class VIENGINE_EXPORT ViProgram
{
public:
	ViProgram(ViShader* vertShader, ViShader* fragShader, std::vector<ViVertexAttribute> aVertexAttributes);

	vi_property_get_named(GLuint, mId, Id);
	vi_property_get_named(std::vector<ViVertexAttribute>, mVertexAttributes, VertexAttributes);

	vi_property_named(bool, mLinked, Linked);
	vi_property_named(bool, mDirty, Dirty);

	//vi_property_get_named(mat4, mObjectMat, ObjectMat);

	//Binds the executing program.
	virtual void Bind(ViVertexBatchInstance& aBindTo);

	virtual void SetUniforms(ViVertexBatchInstance& aBindTo);
	virtual void SetObjectMat(mat4 aObjMat);
	virtual void BindAttributes(ViVertexBatchInstance& aInstance, bool aForceBind = false);

	virtual void BindAttribute(ViVertexAttribute& aAttribute);

protected:

	GLuint GetUniform(std::string name);

	/*void UniformFloat(float val, std::string name);
	void UniformVec4(vec4 val, std::string name);
	void UniformVec3(vec3 val, std::string name);
	void UniformMat4(mat4 val, bool transpose, std::string name);*/

	//Compile and link the shaders to the program. If program 
	void CompileAndLink();

	ViUniformMat4* mUniformObjectMat;

	//mat4 mObjectMat;
	bool mHasObjectMat = true;

private:
	GLuint mId = -1;
	bool mBoundAttributes;

	DISABLE_WARNING_PUSH
	DISABLE_WARNING(4251)
	GLuint mCachedUniform;
	std::string mCachedUniformName;

	ViShader* mVertShader;
	ViShader* mFragShader;

	std::vector<ViVertexAttribute> mVertexAttributes;

	std::unordered_set<ViMesh*> mCachedMeshes;

	const std::string cOBJECT_UNIFORM = "object";
	DISABLE_WARNING_POP
};