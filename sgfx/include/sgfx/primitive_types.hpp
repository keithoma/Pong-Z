#pragma once

#include <cstdint>

namespace sgfx {

struct vec {
};

struct point {
	unsigned x;
	unsigned y;

	bool operator==(const point& p) const noexcept { return x == p.x && y == p.y; }
	bool operator!=(const point& p) const noexcept { return !(*this == p); }
};

/**
  * Retrieves X or Y coordinate depending on compile-time index @p I.
  * @returns X if I == 0, Y if I == 1, compile time error otherwise.
  */
template <const std::size_t I>
constexpr unsigned get(const point& p)
{
	static_assert(I == 0 || I == 1, "I must be 0 or 1.");
	if constexpr (I == 0)
		return p.x;
	else
		return p.y;
}

struct dimension {
	unsigned width;
	unsigned height;

	bool operator==(const dimension& d) const noexcept { return width == d.width && height == d.height; }
	bool operator!=(const dimension& d) const noexcept { return !(*this == d); }
};

struct rectangle {
	point top_left;
	dimension size;
};

}  // namespace sgfx
