// This file is part of the "pong" project, http://github.com/keithoma/pong>
//   (c) 2019-2019 Christian Parpart <christian@parpart.family>
//   (c) 2019-2019 Kei Thoma <thomakmj@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

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
	auto const static inRange = [](auto x, auto begin, auto end) { return begin <= x && x < end; };

	for (int x = 0; x < source.width(); ++x)
		for (int y = 0; y < source.height(); ++y)
			if (inRange(top_left.x + x, 0, target.width()) && inRange(top_left.y + y, 0, target.height()))
				target[top_left + point{x, y}] = source[point{x, y}];

	// TODO: super naive implementation wants to be replaced with its superior
}

}  // namespace sgfx
