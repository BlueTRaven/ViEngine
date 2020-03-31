#pragma once

#include <GL/glew.h>
#include <stdint.h>

#include "ViUtil.h"

template<typename T>
struct ViColor
{
	ViColor();
	ViColor(T r, T g, T b, T a);

	T r;
	T g;
	T b;
	T a;
};

template<typename T>
ViColor<T>::ViColor() :
	r(0),
	g(0),
	b(0),
	a(0)
{ }

template<typename T>
ViColor<T>::ViColor(T r, T g, T b, T a) :
	r(r),
	g(g),
	b(b),
	a(a)
{ }

template struct VIENGINE_EXPORT ViColor<GLclampf>;
//typedef ViColor<GLclampf> ViColorGL;