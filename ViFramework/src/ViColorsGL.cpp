#include "ViColorsGL.h"

ViColorGL::ViColorGL() : ViColor<GLclampf>::ViColor()
{
}

ViColorGL::ViColorGL(GLclampf r, GLclampf g, GLclampf b, GLclampf a) :
	ViColor<GLclampf>::ViColor(r, g, b, a)
{
}
