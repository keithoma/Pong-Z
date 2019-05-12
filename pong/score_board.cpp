#include "score_board.hpp"
#include <sgfx/color.hpp>
#include <sgfx/primitives.hpp>

using namespace sgfx;
using namespace std;

namespace pong {

constexpr int bar_width = 10;
constexpr int bar_padding_width = 10;
constexpr int bar_height = 50;

score_board::score_board(unsigned max_points)
	: canvas{{static_cast<int>(max_points) * (bar_width + bar_padding_width), bar_height}}, max_points_{max_points}, current_points_{0}
{
	sgfx::clear(*this, sgfx::color::black);

	for (size_t n = 1; n <= max_points; ++n)
		draw_bar(n, color::gray);
}

void score_board::draw_bar(int _n, color::rgb_color _color)
{
	auto const top_left = point{(_n - 1) * 20 + 5, 0};
	auto const rect = rectangle{top_left, dimension{10, height()}};

	sgfx::fill(*this, rect, _color);
}

score_board& score_board::operator++()
{
	if (current_points_ < max_points_)
		draw_bar(++current_points_, color::yellow);

	return *this;
}

}  // namespace pong
