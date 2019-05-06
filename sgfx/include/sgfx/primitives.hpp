#pragma once

#include <sgfx/color.hpp>
#include <sgfx/primitive_types.hpp>
#include <sgfx/widget.hpp>

#include <cstdint>

namespace sgfx {

void plot(widget& target, point p, color::rgb_color col);

void clear(widget& target, color::rgb_color col);

void hline(widget& target, point p, std::uint16_t length, color::rgb_color col);
void vline(widget& target, point p, std::uint16_t length, color::rgb_color col);

void fill(widget& target, rectangle rect, color::rgb_color col);

void line(widget& target, point p0, point p1, color::rgb_color col);

}  // namespace sgfx
