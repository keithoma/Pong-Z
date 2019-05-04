#include <algorithm>
#include <iostream>
#include <sgfx/primitives.hpp>

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

	point p = p0;
	unsigned q = 2 * delta_x - delta_y;

	const int xInc = p1.x > p0.x ? +1 : -1;
	const int yInc = p1.y > p0.y ? +1 : -1;

	while (p.x != p1.x) {
		plot(target, p, col);
		p.x += xInc;
		if (q < 0)
			q = q + 2 * delta_y;
		else {
			q = q + 2 * delta_y * delta_x;
			p.y += yInc;
		}
	}
}
