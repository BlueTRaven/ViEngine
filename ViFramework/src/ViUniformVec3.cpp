#include "ViUniformVec3.h"

#include "ViProgram.h"

ViUniformVec3::ViUniformVec3(std::string aName, vec3 aDefault) :
	ViUniform<vec3>::ViUniform(aName, aDefault)
{
}

void ViUniformVec3::Upload(ViProgram * aProgram)
{
	if (mDirty)
	{
		mDirty = false;
		glUniform3f(GetLocation(aProgram), mValue.x, mValue.y, mValue.z);
	}
}
