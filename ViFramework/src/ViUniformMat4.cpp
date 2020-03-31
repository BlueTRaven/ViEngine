#include "ViUniformMat4.h"

#include "ViProgram.h"

ViUniformMat4::ViUniformMat4(std::string aName, mat4 aDefault) :
	ViUniform<mat4>::ViUniform(aName, aDefault)
{
}

void ViUniformMat4::Upload(ViProgram* aProgram)
{
	if (mDirty)
	{
		mDirty = false;
		auto data = glm::value_ptr(mValue);
		glUniformMatrix4fv(GetLocation(aProgram), 1, false, data);
	}
}
