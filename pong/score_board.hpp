#pragma once

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>

namespace pong {

class score_board : public sgfx::canvas {
  public:
	explicit score_board(unsigned max_points);

	score_board& operator++();
	score_board& operator++(int) { return ++*this; }

  private:
	void draw_bar(int _n, sgfx::color::rgb_color _color);

  private:
	unsigned max_points_;
	unsigned current_points_;
};

}  // namespace pong