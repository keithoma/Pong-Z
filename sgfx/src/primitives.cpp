#include <algorithm>
#include <iostream>

#include <sgfx/primitives.hpp>

using namespace std;

namespace sgfx {

void plot(widget& target, point p, color::rgb_color col)
{
	p.x = min(p.x, static_cast<unsigned>(target.width()) - 1);
	p.y = min(p.y, static_cast<unsigned>(target.height()) - 1);

	target.pixels()[p.y * target.width() + p.x] = col;
}

void clear(widget& target, color::rgb_color col)
{
	fill(begin(target.pixels()), end(target.pixels()), col);
}

void hline(widget& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.y < target.height()) {
		const unsigned xEnd = min(p.x + length, static_cast<unsigned>(target.width()));
		while (p.x < xEnd) {
			plot(target, p, col);
			++p.x;
		}
	}
}

void vline(widget& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.x < target.width()) {
		const unsigned yEnd = min(p.y + length, static_cast<unsigned>(target.height()));
		while (p.y < yEnd) {
			plot(target, p, col);
			++p.y;
		}
	}
}

void fill(widget& target, rectangle rect, color::rgb_color col)
{
	if (rect.top_left == point{0, 0} && rect.size == dimension{target.width(), target.height()})
		clear(target, col);
	else
		for (int i = 0; i < rect.size.height; ++i)
			hline(target, {rect.top_left.x, rect.top_left.y + i}, rect.size.width, col);
}

void line(widget& target, point p0, point p1, color::rgb_color col)
{
	auto line_low = [](widget& target, point p0, point p1, color::rgb_color col) {
		int dx = p1.x - p0.x;
		int dy = p1.y - p0.y;
		int yi = 1;

		if (dy < 0) {
			yi = -1;
			dy = -dy;
		}

		int big_d = 2 * dy - dx;
		unsigned y = p0.y;

		for (unsigned x = p0.x; x < p1.x; ++x) {
			plot(target, {x, y}, col);

			if (big_d > 0) {
				y += yi;
				big_d -= 2 * dx;
			}

			big_d += 2 * dy;
		}
	};

	static auto line_high = [](widget& target, point p0, point p1, color::rgb_color col) {
		int dx = p1.x - p0.x;
		int dy = p1.y - p0.y;
		int xi = 1;

		if (dx < 0) {
			xi = -1;
			dx = -dx;
		}

		int big_d = 2 * dx - dy;
		unsigned x = p0.x;

		for (unsigned y = p0.y; y < p1.y; ++y) {
			plot(target, {x, y}, col);

			if (big_d > 0) {
				x += xi;
				big_d -= 2 * dy;
			}

			big_d += 2 * dx;
		}
	};

	// optimize first

	// see Bresenham's line algorithm
	if (labs(p1.y - p0.y) < labs(p1.x - p0.x)) {
		if (p0.x > p1.x)
			line_low(target, p1, p0, col);
		else
			line_low(target, p0, p1, col);
	}
	else {
		if (p0.y > p1.y)
			line_high(target, p1, p0, col);
		else
			line_high(target, p0, p1, col);
	}
}

}  // namespace sgfx