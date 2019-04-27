#ifndef SGFX_CANVAS_H
#define SGFX_CANVAS_H

#include <sgfx/color.hpp>
#include <sgfx/primitive_types.hpp>
#include <sgfx/window.hpp>


namespace sgfx
{
	class canvas
	{
		public:
		explicit canvas(dimension size);
		
		std::uint16_t width() const;
		std::uint16_t height() const;
		
		color::rgb_color* pixels();
		const color::rgb_color* pixels() const;
		
	};
	
	void draw(window& target, const canvas& img, point top_left);
}

#endif
