#ifndef SGFX_PRIMITIVE_TYPES_H
#define SGFX_PRIMITIVE_TYPES_H

#include <cstdint>

namespace sgfx
{
	struct vec
	{
	};
	
	struct point
	{
	};
	
	struct dimension
	{
	};
	
	struct rectangle
	{
		point top_left;
		dimension size;
	};
}

#endif
