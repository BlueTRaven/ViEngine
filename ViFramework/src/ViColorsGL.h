#pragma once

#include "ViUtil.h"
#include "ViColor.h"

struct VIENGINE_EXPORT ViColorGL : public ViColor<GLclampf>
{
	ViColorGL();

	ViColorGL(GLclampf r, GLclampf g, GLclampf b, GLclampf a);

	//Returns a normalized vec4 (converted from range 0-255 to 0-1.)
	vec4 ToVec4() const
	{
		return vec4(r, g, b, a);
	}

	vec4i ToVec4i() const
	{
		return vec4i(r, g, b, a);
	}
};

namespace vicolors
{
	const ViColorGL WHITE = ViColorGL(1.0, 1.0, 1.0, 1.0);
	const ViColorGL BLACK = ViColorGL(0.0, 0.0, 0.0, 1.0);
	const ViColorGL RED = ViColorGL(1.0, 0.0, 0.0, 1.0);
	const ViColorGL GREEN = ViColorGL(0.0, 1.0, 0.0, 1.0);
	const ViColorGL BLUE = ViColorGL(0, 0.0, 1.0, 1.0);
	const ViColorGL YELLOW = ViColorGL(1.0, 1.0, 0.0, 1.0);
}