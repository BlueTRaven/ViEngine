#pragma once

#include "ViTransform.h"
#include "ViUtil.h"

#include "ProgramGeneric.h"
#include "ProgramLitGeneric.h"

class ViVertexBatch;

namespace vigame
{
	class Camera
	{
	public:
		Camera(ViTransform aStartTransform);

		void Update(float aDeltaTime);
		void LateUpdate(float aDeltaTime);

		void Draw(ViVertexBatch* aBatch);

		void Transform(vec3 aPosition, vec3 aRotation);

		vi_property_get_named(ViTransform, mTransform, Transform);
		vi_property_set_named(ViTransform, mTransform, Transform);

	private:
		ViTransform mTransform;

		ViProgramGeneric* mProgramGeneric;
		ProgramLitGeneric* mProgramLitGeneric;
		ViProgramGeneric* mProgramGenericFullBright;
	};
}