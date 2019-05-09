#include <sgfx/canvas.hpp>
#include <sgfx/primitives.hpp>

namespace sgfx {

canvas canvas::colored(dimension size, color::rgb_color col)
{
	canvas c{size};
	clear(c, col);
	return c;
}

void draw(widget& target, const canvas& source, point top_left)
{
	for (int x = 0; x < source.width(); ++x)
		for (int y = 0; y < source.height(); ++y)
			if (top_left.x + x < source.width() && top_left.y + y < source.height())
				target[top_left + point{x, y}] = source[point{x, y}];

	// TODO: super naive implementation wants to be replaced with its superior
}

}  // namespace sgfx