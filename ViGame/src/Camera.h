#pragma once

#include "ViTransform.h"
#include "ViUtil.h"

#include "ProgramGeneric.h"

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

		vi_property_get_named(ViTransform, mTransform, Transform);
		vi_property_set_named(ViTransform, mTransform, Transform);

	private:
		ViTransform mTransform;

		ViProgramGeneric* mProgramGeneric;
		ViProgramGeneric* mProgramGenericFullBright;
	};
}