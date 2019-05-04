#include <sgfx/primitives.hpp>
#include <iostream>

void sgfx::plot(window& target, point p, color::rgb_color col)
{
    // [Kei]: does work
	p.x %= target.width();
	p.y %= target.height();

	target.pixels()[p.y * target.width() + p.x] = col;
}

void sgfx::clear(window& target, color::rgb_color col)
{
    // [Kei]: not sure about the end point of the for iteration
    // [Kei]: does work
    int window_area = target.width() * target.height();

    for (int i = 0; i < window_area; ++i) {
        target.pixels()[i] = col;
    }
}

void sgfx::hline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
    // [Kei]: does work
    // [Kei]: this will not yet take in account if the line goes over the boarder
    for (int i = 0; i < length; ++i) {
        ++p.x;
        sgfx::plot(target, p, col);
    }
}

void sgfx::vline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
    // [Kei]: does work
    // [Kei]: this will not yet take in account if the line goes over the boarder
    for (int i = 0; i < length; ++i) {
        ++p.y;
        sgfx::plot(target, p, col);
    }
}

void sgfx::fill(window& target, rectangle rect, color::rgb_color col)
{
    // [Kei]: no idea if this works
    point p = {rect.top_left};

    for (int i = 0; i < rect.size.height; ++i) {
        ++p.y;
        sgfx::hline(target, p, rect.size.width , col);
    }
}

void sgfx::line(window& target, point p0, point p1, color::rgb_color col)
{
    // [Kei]: does not work propperly yet
    int delta_x = p0.x - p1.x;
    int delta_y = p0.y - p1.y;
    float delta_err = abs(delta_y / delta_x);
    float err = 0.0;
    int y = p0.y;

    for ( int x = p0.x; x < p1.x; ++x ) {
        sgfx::plot(target, {x, y}, col);
        err = err + delta_err;
        if (err >= 0.5) {
            y = y + (delta_y > 0) - (delta_y < 0);
            err = err - 1;
        }
    }
}
