#include "ViUniformFloat.h"

#include "ViProgram.h"

ViUniformFloat::ViUniformFloat(std::string aName, float aDefault) :
	ViUniform<float>::ViUniform(aName, aDefault)
{
}

void ViUniformFloat::Upload(ViProgram * aProgram)
{
	if (mDirty)
	{
		mDirty = false;
		glUniform1f(GetLocation(aProgram), mValue);
	}
}
