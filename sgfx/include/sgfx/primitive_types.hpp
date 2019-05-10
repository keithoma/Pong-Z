#pragma once

#include <cstddef>

namespace sgfx {

struct point {
	int x;
	int y;

	bool operator==(const point& p) const noexcept { return x == p.x && y == p.y; }
	bool operator!=(const point& p) const noexcept { return !(*this == p); }

	template <const std::size_t A, const std::size_t B>
	static constexpr point indexed(int a, int b)
	{
		static_assert(A == 0 || A == 1, "A must be either 0 or 1.");
		static_assert(B == 0 || B == 1, "B must be either 0 or 1.");
		static_assert(A != B, "A must not equal to B.");
		if constexpr (A == 0)
			return point{a, b};
		else
			return point{b, a};
	}
};

constexpr point operator+(point const& a, point const& b)
{
	return point{a.x + b.x, a.y + b.y};
}

constexpr point operator-(point const& a, point const& b)
{
	return point{a.x - b.x, a.y - b.y};
}

template <const std::size_t I>
constexpr int const& get(point const& p)
{
	static_assert(I == 0 || I == 1, "I must be 0 (for X) or 1 (for Y).");
	if constexpr (I == 0)
		return p.x;
	else
		return p.y;
}

template <const std::size_t I>
constexpr int& get(point& p)
{
	static_assert(I == 0 || I == 1, "I must be 0 (for X) or 1 (for Y).");
	if constexpr (I == 0)
		return p.x;
	else
		return p.y;
}

// vec is a 2 dimensional field, so is point.
using vec = point;

struct dimension {
	int width;
	int height;

	bool operator==(const dimension& d) const noexcept { return width == d.width && height == d.height; }
	bool operator!=(const dimension& d) const noexcept { return !(*this == d); }
};

struct rectangle {
	point top_left;
	dimension size;
};

}  // namespace sgfx
