#pragma once

#include "ViTransform.h"
#include "ViUtil.h"

#include "ProgramGeneric.h"
#include "ProgramUnlitGeneric.h"
#include "ProgramLitGeneric.h"

class ViVertexBatch;

namespace vigame
{
	class Camera
	{
	public:
		enum CameraControlMode
		{
			cCAMERA_CONTROLLED,
			cTARGET_CONTROLLED,	//Set a target and it is eased to	
			cCUSTOM_CONTROLLED,	//Controlled by some other function, not defined here. Represents no additional functionality in this class.
		};

		Camera(ViTransform aStartTransform);

		void Update(float aDeltaTime);
		void LateUpdate(float aDeltaTime);

		void Draw(ViVertexBatch* aBatch);

		vi_property_get_named(ViTransform, mTransform, Transform);
		vi_property_set_named(ViTransform, mTransform, Transform);

		vi_property_named(CameraControlMode, mControlMode, ControlMode);
		vi_property_named(vec3, mTargetPosition, TargetPosition);
		vi_property_named(vec3, mTargetRotation, TargetRotation);
		vi_property_named(float, mTargetEase, TargetEase);

		vi_property_named(ProgramLitGeneric::DiffuseLight, mDiffuseLight, DiffuseLight);
		vi_property_named(ProgramLitGeneric::RadialFog, mRadialFog, RadialFog);

	private:
		ViTransform mTransform;

		ViProgramGeneric* mProgramGeneric;
		ProgramUnlitGeneric* mProgramUnlitGeneric;
		ProgramLitGeneric* mProgramLitGeneric;
		ViProgramGeneric* mProgramGenericFullBright;
	};
}