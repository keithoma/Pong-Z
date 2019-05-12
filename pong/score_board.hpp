// This file is part of the "pong" project, http://github.com/keithoma/pong>
//   (c) 2019-2019 Christian Parpart <christian@parpart.family>
//   (c) 2019-2019 Kei Thoma <thomakmj@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>

namespace pong {

class score_board : public sgfx::canvas {
  public:
	explicit score_board(unsigned max_points);

	score_board& operator++();
	score_board& operator++(int) { return ++*this; }

	score_board& operator=(unsigned);

	void redraw();

  private:
	void draw_bar(unsigned _n, sgfx::color::rgb_color _color);

  private:
	unsigned max_points_;
	unsigned current_points_;
};

}  // namespace pong
