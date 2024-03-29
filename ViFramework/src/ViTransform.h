#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "ViUtil.h"

class VIENGINE_EXPORT ViTransform
{
public:
	ViTransform();
	ViTransform(glm::vec3 aPosition, glm::vec3 aRotation, glm::vec3 aScale);

	vec3 Forward();
	vec3 Left();
	vec3 Right();
	vec3 Backwards();
	vec3 Up();
	vec3 Down();

	mat4 Matrix();

	void Translate(vec3 direction);
	void Rotate(vec3 rotation);

	bool operator ==(ViTransform const &obj);
	bool operator !=(ViTransform const &obj);

	void SetPosition(vec3 aPosition);
	void SetRotation(vec3 aRotation);
	void SetScale(vec3 aScale);

	vi_property_get_named(vec3, mPosition, Position);
	vi_property_get_named(vec3, mRotation, Rotation);
	vi_property_get_named(vec3, mScale, Scale);

	vi_property_named(bool, mChanged, Changed);

	const static  ViTransform None;

	//Creates a ViTransform positioned at aPosition.
	static ViTransform Positioned(vec3 aPosition);
	//Creates a ViTransform rotated at aRotation.
	static ViTransform Rotated(vec3 aRotation);
	//Creates a ViTransform scaled at aScale.
	static ViTransform Scaled(vec3 aScale);

private:
	mat4 mCachedTransform;

	vec3 mPosition;
	vec3 mRotation;
	vec3 mScale;

	bool mSetScale;
	bool mSetRotation;
	bool mSetPosition;
};