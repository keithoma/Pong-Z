#ifndef SGFX_PRIMITIVES_H
#define SGFX_PRIMITIVES_H

#include <sgfx/primitive_types.hpp>
#include <sgfx/window.hpp>

#include <cstdint>

namespace sgfx
{
	void plot(window& target, point p, color::rgb_color col);
	
	void clear(window& target, color::rgb_color col);
	
	void hline(window& target, point p, std::uint16_t length, color::rgb_color col);
	void vline(window& target, point p, std::uint16_t length, color::rgb_color col);
	
	void fill(window& target, rectangle rect, color::rgb_color col);
	
	void line(window& target, point p0, point p1, color::rgb_color col);
	
}

#endif
