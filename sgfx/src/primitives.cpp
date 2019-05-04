#include <sgfx/primitives.hpp>
#include <iostream>
#include <algorithm>

using namespace std;

void sgfx::plot(window& target, point p, color::rgb_color col)
{
	p.x = min(p.x, static_cast<unsigned>(target.width()) - 1);
	p.y = min(p.y, static_cast<unsigned>(target.height()) - 1);

	target.pixels()[p.y * target.width() + p.x] = col;
}

void sgfx::clear(window& target, color::rgb_color col)
{
	fill(begin(target.pixels()), end(target.pixels()), col);
}

void sgfx::hline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.y < target.height()) {
		const unsigned xEnd = min(p.x + length, static_cast<unsigned>(target.width()));
		while (p.x < xEnd) {
			plot(target, p, col);
			++p.x;
		}
	}
}

void sgfx::vline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.x < target.width()) {
		const unsigned yEnd = min(p.y + length, static_cast<unsigned>(target.height()));
		while (p.y < yEnd) {
			plot(target, p, col);
			++p.y;
		}
	}
}

void sgfx::fill(window& target, rectangle rect, color::rgb_color col)
{
	if (rect.top_left == point{0, 0} && rect.size == dimension{target.width(), target.height()})
		clear(target, col);
	else
		for (int i = 0; i < rect.size.height; ++i)
			hline(target, {rect.top_left.x, rect.top_left.y + i}, rect.size.width, col);
}

void sgfx::line(window& target, point p0, point p1, color::rgb_color col)
{
	// [Kei]: does not work propperly yet
	const int delta_x = p0.x - p1.x;
	const int delta_y = p0.y - p1.y;
	const float delta_err = abs(delta_y / delta_x);
	float err = 0.0;
	unsigned y = p0.y;

	for (unsigned x = p0.x; x < p1.x; ++x) {
		plot(target, {x, y}, col);
		err = err + delta_err;
		if (err >= 0.5) {
			y = y + (delta_y > 0) - (delta_y < 0);
			err = err - 1;
		}
	}
}
