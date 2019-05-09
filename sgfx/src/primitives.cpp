#include <sgfx/primitives.hpp>

#include <algorithm>
#include <iostream>
#include <tuple>
#include <functional>
#include <cassert>

using namespace std::placeholders;
using namespace std;

namespace sgfx {

void plot(widget& target, point p, color::rgb_color col)
{
	p.x = min(p.x, static_cast<int>(target.width()) - 1);
	p.y = min(p.y, static_cast<int>(target.height()) - 1);

	target.pixels()[p.y * target.width() + p.x] = col;
}

void clear(widget& target, color::rgb_color col)
{
	fill(begin(target.pixels()), end(target.pixels()), col);
}

void hline(widget& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.y < target.height()) {
		const int xEnd = min(p.x + length, static_cast<int>(target.width()));
		while (p.x < xEnd) {
			plot(target, p, col);
			++p.x;
		}
	}
}

void vline(widget& target, point p, std::uint16_t length, color::rgb_color col)
{
	if (p.x < target.width()) {
		const int yEnd = min(p.y + length, static_cast<int>(target.height()));
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
		for (unsigned i = 0; i < rect.size.height; ++i)
			hline(target, {rect.top_left.x, rect.top_left.y + static_cast<int>(i)}, rect.size.width, col);
}

/**
 * Breseham Algorithm line point generator.
 *
 * @param A indexed access to point coordinate, where 0 means X and 1 means Y.
 * @param B indexed access to point coordinate, where 0 means X and 1 means Y.
 *
 * @param p0 first point
 * @param p1 second point
 * @param drawer function to invoke for each generated point.
 */
template <const size_t A, const size_t B>
constexpr void breseham(point p0, point p1, std::function<void(point const&)> sink) {
	static_assert(A == 0 || A == 1, "A must be 0 (for X) or 1 (for Y).");
	static_assert(B == 0 || B == 1, "B must be 0 (for X) or 1 (for Y).");
	static_assert(A != B, "A must not be equal to B.");
	assert(p0.y <= p1.y && "Must draw from top to bottom (p0.y <= p1.y).");

	// compute delta x/y and the increment value for the walking coordinate
	auto const [delta, increment] = [](auto p0, auto p1) {
		if (const auto delta = p1 - p0; get<B>(delta) >= 0)
			return make_tuple(delta, +1);
		else {
			const auto sgn = int{A == 0 /*A is X*/ ? +1 : -1};
			return make_tuple(point{sgn * delta.x, -sgn * delta.y}, -1);
		}
	}(p0, p1);

	auto big_d = int{2 * get<B>(delta) - get<A>(delta)};
	auto p = p0;
	while (get<A>(p) <= get<A>(p1)) {
		sink(p);
		if (big_d > 0) {
			big_d -= 2 * get<A>(delta);
			get<B>(p) += increment;
		}
		++get<A>(p);
		big_d += 2 * get<B>(delta);
	}
}

void line(widget& target, point p0, point p1, color::rgb_color col)
{
	if (p0 == p1) {
		// point
		plot(target, p0, col);
	}
	else if (p0.y == p1.y) {
		// vertical line
		hline(target, p0, labs(p1.x - p0.x), col);
	}
	else if (p0.x == p1.x) {
		// horizontal line
		vline(target, p0, labs(p1.y - p0.y), col);
	}
	else if (labs(p1.y - p0.y) < labs(p1.x - p0.x)) {
		// non-trivial line: with the power of Bresenham
		if (p0.x > p1.x)
			breseham<0, 1>(p1, p0, bind(plot, ref(target), _1, col));
		else
			breseham<0, 1>(p0, p1, bind(plot, ref(target), _1, col));
	}
	else {
		// non-trivial line: with the power of Bresenham
		if (p0.y > p1.y)
			breseham<1, 0>(p1, p0, bind(plot, ref(target), _1, col));
		else
			breseham<1, 0>(p0, p1, bind(plot, ref(target), _1, col));
	}
}

}  // namespace sgfx
