#include <sgfx/primitives.hpp>

void sgfx::plot(window& target, point p, color::rgb_color col)
{
    // [Kei]: added the three lines, not sure if this is right
	p.x %= main_window.width();
	p.y %= main_window.height();

	main_window.pixels()[y * main_window.width() + x] = color::white;
}

void sgfx::clear(window& target, color::rgb_color col)
{
}

void sgfx::hline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
}

void sgfx::vline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
}

void sgfx::fill(window& target, rectangle rect, color::rgb_color col)
{
}

void sgfx::line(window& target, point p0, point p1, color::rgb_color col)
{
}
