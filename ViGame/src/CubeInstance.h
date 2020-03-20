#pragma once

#include <stdint.h>

namespace vigame
{
	typedef uint8_t cubeid;	//define a type for cube ids separately since this can change later on

	struct CubeInstance
	{
		CubeInstance(cubeid aId);
		cubeid mId;
	};
}