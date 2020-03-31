#include "ViUniformVec4.h"

#include "ViProgram.h"

ViUniformVec4::ViUniformVec4(std::string aName, vec4 aDefault) :
	ViUniform<vec4>::ViUniform(aName, aDefault)
{
}

void ViUniformVec4::Upload(ViProgram * aProgram)
{
	if (mDirty)
	{
		mDirty = false;
		glUniform4f(GetLocation(aProgram), mValue.x, mValue.y, mValue.z, mValue.w);
	}
}
