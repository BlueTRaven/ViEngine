#pragma once

#include <string>

#include "ViUtil.h"

class VIENGINE_EXPORT ViVertexAttribute
{
public:
	ViVertexAttribute(std::string name, uint16_t elements, size_t size, void* offset) :
		name(name),
		elements(elements),
		size(size),
		offset(offset)
	{ }

	vi_property(GLuint, id);
		
	vi_property(std::string, name);
	vi_property(uint16_t, elements);
	vi_property(size_t, size);
	vi_property(void*, offset);
};