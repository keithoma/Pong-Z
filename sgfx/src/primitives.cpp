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

    for( int i = 0; i < window_area; ++i ) {
        target.pixels()[i] = col;
    }
}

void sgfx::hline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
    // [Kei]: does work
    // [Kei]: this will not yet take in account if the line goes over the boarder
    for( int i = 0; i < length; ++i) {
        ++p.x;
        sgfx::plot(target, p, col);
    }
}

void sgfx::vline(window& target, point p, std::uint16_t length, color::rgb_color col)
{
    // [Kei]: does work
    // [Kei]: this will not yet take in account if the line goes over the boarder
    for( int i = 0; i < length; ++i) {
        ++p.y;
        sgfx::plot(target, p, col);
    }
}

void sgfx::fill(window& target, rectangle rect, color::rgb_color col)
{
    // [Kei]: no idea if this works
    /* for( int i = 0; i < rectangle.dimension.height; ++i) {
        p.y %= target.height() + 1;
        sgfx::hline(target, p, rectangle.dimension.width , col);
    } */
}

void sgfx::line(window& target, point p0, point p1, color::rgb_color col)
{
}
