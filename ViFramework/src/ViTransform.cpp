#include "ViTransform.h"

ViTransform::ViTransform() :
	mPosition(glm::vec3(0, 0, 0)),
	mRotation(glm::vec3(0, 0, 0)),
	mScale(glm::vec3(1, 1, 1)),
	mSetPosition(false),
	mSetRotation(false),
	mSetScale(false)
{
}

ViTransform::ViTransform(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale) :
	mPosition(aPosition),
	mRotation(aRotation),
	mScale(aScale),
	mSetPosition(false),
	mSetRotation(false),
	mSetScale(false)
{

}

glm::vec3 ViTransform::Forward()
{
	glm::mat4 rotm = glm::rotate(rotm, glm::radians(mRotation.x), glm::vec3(1.0, 0.0, 0.0));
	rotm = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.y), glm::vec3(0.0, 1.0, 0.0));
	glm::vec4 forward = glm::vec4(0.0, 0.0, -1.0, 1.0);
	glm::vec4 out = forward * rotm;
	return glm::normalize(glm::vec3(out.x, out.y, out.z));
}

glm::vec3 ViTransform::Left()
{
	return Right() * glm::vec3(-1.0, -1.0, -1.0);
}

glm::vec3 ViTransform::Right()
{
	glm::mat4 rotm = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.y), glm::vec3(0.0, 1.0, 0.0));
	glm::vec4 right = glm::vec4(1.0, 0.0, 0.0, 1.0);
	glm::vec4 out = right * rotm;
	return glm::normalize(glm::vec3(out.x, out.y, out.z));
}

glm::vec3 ViTransform::Backwards()
{
	return Forward() * glm::vec3(-1.0, -1.0, -1.0);
}

glm::vec3 ViTransform::Up()
{
	glm::mat4 rotm = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.x), glm::vec3(0.0, 1.0, 0.0));
	glm::vec4 right = glm::vec4(0.0, 1.0, 0.0, 1.0);
	glm::vec4 out = right * rotm;
	return glm::normalize(glm::vec3(out.x, out.y, out.z));
}

glm::vec3 ViTransform::Down()
{
	return Up() * glm::vec3(-1.0, -1.0, -1.0);
}

glm::mat4 ViTransform::Matrix()
{
	if (!mChanged)
		return mCachedTransform;

	//Short-circuits
	//If we've only set position, scale, or rotation, we don't have to bother with multiplying anything together.
	if (mSetPosition && !mSetScale && !mSetRotation)
	{
		mChanged = false;
		mCachedTransform = glm::translate(glm::identity<glm::mat4>(), mPosition);
		return mCachedTransform;
	}

	if (mSetRotation && !mSetScale && !mSetPosition)
	{
		glm::mat4 rotX = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.x), glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.y), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.z), glm::vec3(0.0, 0.0, 1.0));

		mChanged = false;
		mCachedTransform = rotX * rotY * rotZ;
		return mCachedTransform;
	}

	if (mSetScale && !mSetRotation && !mSetPosition)
	{
		mChanged = false;
		mCachedTransform = glm::scale(glm::identity<glm::mat4>(), mScale);
		return mCachedTransform;
	}

	glm::mat4 scale = glm::identity<glm::mat4>();
	if (mSetScale)
		scale = glm::scale(scale, mScale);
	
	glm::mat4 rot = glm::identity<glm::mat4>();
	if (mSetRotation)
	{
		glm::mat4 rotX = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.x), glm::vec3(1.0, 0.0, 0.0));
		glm::mat4 rotY = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.y), glm::vec3(0.0, 1.0, 0.0));
		glm::mat4 rotZ = glm::rotate(glm::identity<glm::mat4>(), glm::radians(mRotation.z), glm::vec3(0.0, 0.0, 1.0));

		rot = rotX * rotY * rotZ;
	}

	glm::mat4 trans = glm::identity<glm::mat4>();
	if (mSetPosition)
		trans = glm::translate(glm::identity<glm::mat4>(), mPosition);

	mChanged = false;
	mCachedTransform = trans;
	return scale * rot * trans;
}

void ViTransform::Translate(glm::vec3 direction)
{
	SetPosition(GetPosition() + direction);
	mChanged = true;
	mSetPosition = true;
}

void ViTransform::Rotate(glm::vec3 rotation)
{
	SetRotation(GetRotation() + rotation);
	mChanged = true;
	mSetRotation = true;
}

bool ViTransform::operator==(ViTransform const& obj)
{
	return this == &obj;
}

bool ViTransform::operator!=(ViTransform const & obj)
{
	return this != &obj;
}

void ViTransform::SetPosition(vec3 aPosition)
{
	mPosition = aPosition;
	mSetPosition = true;
	mChanged = true;
}

void ViTransform::SetRotation(vec3 aRotation)
{
	mRotation = aRotation;
	mSetRotation = true;
	mChanged = true;
}

void ViTransform::SetScale(vec3 aScale)
{
	mScale = aScale;
	mSetScale = true;
	mChanged = true;
}

ViTransform ViTransform::Positioned(glm::vec3 aPosition)
{
	ViTransform trans;
	trans.Translate(aPosition);

	return trans;
}

ViTransform ViTransform::Rotated(glm::vec3 aRotation)
{
	ViTransform trans;
	trans.SetRotation(aRotation);

	return trans;
}

ViTransform ViTransform::Scaled(glm::vec3 aScale)
{
	ViTransform trans;
	trans.SetScale(aScale);

	return trans;
}

const ViTransform ViTransform::None = ViTransform();
