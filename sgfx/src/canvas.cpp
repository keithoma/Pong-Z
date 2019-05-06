#include <sgfx/canvas.hpp>

void sgfx::draw(widget& target, const canvas& source, point top_left)
{
	// TODO: kopiere das source `source` nach `target` an top/left posi rein.
	for (unsigned x = top_left.x; x < top_left.x + source.width(); ++x)
		for (unsigned y = top_left.y; y < top_left.y + source.height(); ++y)
			target.pixels()[x * target.width() + y] = source.pixels()[x * source.width() + y];

	// TODO: der kot ist ultra naiv implementiert. mach mich schlau. :-D
}
