#include <algorithm>
#include <iostream>

#include <sgfx/primitives.hpp>

using namespace std;

void sgfx::plot(widget& target, point p, color::rgb_color col)
{

	p.x = min(p.x, static_cast<unsigned>(target.width()) - 1);
	p.y = min(p.y, static_cast<unsigned>(target.height()) - 1);

	target.pixels()[p.y * target.width() + p.x] = col;
}

void sgfx::clear(widget& target, color::rgb_color col)
{
	fill(begin(target.pixels()), end(target.pixels()), col);
}

void sgfx::hline(widget& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.y < target.height()) {
		const unsigned xEnd = min(p.x + length, static_cast<unsigned>(target.width()));
		while (p.x < xEnd) {
			plot(target, p, col);
			++p.x;
		}
	}
}

void sgfx::vline(widget& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.x < target.width()) {
		const unsigned yEnd = min(p.y + length, static_cast<unsigned>(target.height()));
		while (p.y < yEnd) {
			plot(target, p, col);
			++p.y;
		}
	}
}

void sgfx::fill(widget& target, rectangle rect, color::rgb_color col)
{
	if (rect.top_left == point{0, 0} && rect.size == dimension{target.width(), target.height()})
		clear(target, col);
	else
		for (int i = 0; i < rect.size.height; ++i)
			hline(target, {rect.top_left.x, rect.top_left.y + i}, rect.size.width, col);
}

// added some auxiliary functions, 'line_low()' and 'line_high()'; they are only used in 'line()'

void line_low(sgfx::widget& target, sgfx::point p0, sgfx::point p1, sgfx::color::rgb_color col)
{
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
}

void line_high(sgfx::widget& target, sgfx::point p0, sgfx::point p1, sgfx::color::rgb_color col)
{
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
}

void sgfx::line(widget& target, point p0, point p1, color::rgb_color col)
{
	// optimize first

    // see Bresenham's line algorithm
	if (labs(p1.y - p0.y) < labs(p1.x - p0.x)) {
		if (p0.x > p1.x)
			line_low(target, {p1.x, p1.y}, {p0.x, p0.y}, col);
		else
			line_low(target, {p0.x, p0.y}, {p1.x, p1.y}, col);
	}
	else {
		if (p0.y > p1.y)
			line_high(target, {p1.x, p1.y}, {p0.x, p0.y}, col);
		else
			line_high(target, {p0.x, p0.y}, {p1.x, p1.y}, col);
	}
}
