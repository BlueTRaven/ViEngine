#pragma once

#include <string>
#include <GL/glew.h>

#include "ViUtil.h"

class ViProgram;

template <typename T>
class ViUniform
{
public:
	ViUniform(std::string aName, T aDefault);

	//Sets the current data and marks the program as dirty. Does not set anything on the program's gl side. See Upload for that.
	virtual void Set(ViProgram* aProgram, T aTo);

	T Get();

	//Uploads the current data to the program
	virtual void Upload(ViProgram* aProgram) = 0;

protected:
	string mName;

	//Gets the location of the uniform in the program.
	GLuint GetLocation(ViProgram* aProgram);

	T mValue;
	bool mDirty;

private:
	bool mCached;
	GLuint mCachedUniformLocation;
};

template <typename T>
ViUniform<T>::ViUniform(std::string aName, T aDefault) :
	mName(aName),
	mCached(false),
	mDirty(true),	//since we set the default value, we want to upload it, so dirty is set by default.
	mValue(aDefault)
{ };

template <typename T>
void ViUniform<T>::Set(ViProgram* aProgram, T aTo)
{
	GetLocation(aProgram);
	mDirty = true;
	mValue = aTo;
};

template <typename T>
T ViUniform<T>::Get()
{
	return mValue;
};

template <typename T>
GLuint ViUniform<T>::GetLocation(ViProgram* aProgram)
{
	if (mCached)
		return mCachedUniformLocation;

	if (!aProgram->GetLinked())
	{
		printf("Error: Program has not been linked - uniform %s cannot be found.\n", mName.c_str());
		mCached = false;
		return 0;
	}

	GLint uniform = glGetUniformLocation(aProgram->GetId(), mName.c_str());
	if (uniform <= -1)
	{
		printf("Error: Could not find uniform %s position.\n", mName.c_str());
		mCached = false;
		return 0;
	}

	mCached = true;
	mCachedUniformLocation = uniform;

	return uniform;
};