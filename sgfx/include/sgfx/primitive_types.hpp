#pragma once

#include <cstddef>
#include <string>
#include <cstdio>

namespace sgfx {

/**
 * Represents a 2-dimensional vector in space.
 */
template <typename F = int>
struct vec {
	F x;
	F y;

	constexpr bool operator==(const vec<F>& p) const noexcept { return x == p.x && y == p.y; }
	constexpr bool operator!=(const vec<F>& p) const noexcept { return !(*this == p); }

	/**
	 * Constructs a vec by explicitly indexing the coordinates that are going to be assigned.
	 *
	 * For (A, B) == (0, 1) the vec is constructed as {a, b}
	 * For (A, B) == (1, 0) the vec is constructed as {b, a}
	 */
	template <const std::size_t A, const std::size_t B>
	static constexpr vec<F> indexed(int a, int b)
	{
		static_assert(A == 0 || A == 1, "A must be either 0 or 1.");
		static_assert(B == 0 || B == 1, "B must be either 0 or 1.");
		static_assert(A != B, "A must not equal to B.");
		if constexpr (A == 0)
			return vec<F>{a, b};
		else
			return vec<F>{b, a};
	}
};

template <typename F>
constexpr vec<F> operator*(vec<F> const& a, int s)
{
	return vec<F>{a.x * s, a.y * s};
}

template <typename F>
constexpr vec<F> operator*(vec<F> const& a, double s)
{
	return vec<F>{static_cast<int>(a.x * s), static_cast<int>(a.y * s)};
}

template <typename F>
constexpr vec<F> operator+(vec<F> const& a, vec<F> const& b)
{
	return vec<F>{a.x + b.x, a.y + b.y};
}

template <typename F>
constexpr vec<F> operator-(vec<F> const& a, vec<F> const& b)
{
	return vec<F>{a.x - b.x, a.y - b.y};
}

template <const std::size_t I, typename F>
constexpr int const& get(vec<F> const& p)
{
	static_assert(I == 0 || I == 1, "I must be 0 (for X) or 1 (for Y).");
	if constexpr (I == 0)
		return p.x;
	else
		return p.y;
}

template <const std::size_t I, typename F>
constexpr int& get(vec<F>& p)
{
	static_assert(I == 0 || I == 1, "I must be 0 (for X) or 1 (for Y).");
	if constexpr (I == 0)
		return p.x;
	else
		return p.y;
}

template <typename F>
inline std::string to_string(vec<F> const& p)
{
	char buf[80];
	return std::string(buf, snprintf(buf, sizeof(buf), "{%d, %d}", p.x, p.y));
}

using point = vec<int>;

struct dimension {
	int width;
	int height;

	constexpr bool operator==(const dimension& d) const noexcept
	{
		return width == d.width && height == d.height;
	}

	constexpr bool operator!=(const dimension& d) const noexcept { return !(*this == d); }

	template <typename F>
	constexpr operator vec<F>() const noexcept { return vec<F>{width, height}; }
};

constexpr dimension operator/(dimension const& dim, int div)
{
	return dimension{dim.width / div, dim.height / div};
}

struct rectangle {
	point top_left;
	dimension size;

	constexpr int top() const noexcept { return top_left.y; }
	constexpr int bottom() const noexcept { return top_left.y + size.height; }
	constexpr int left() const noexcept { return top_left.x; }
	constexpr int right() const noexcept { return top_left.x + size.width; }
};

/**
 * Tests whether or not rectangle a is intersecting with rectangle b.
 *
 * @retval true they are intersecting with each other.
 * @retval false they're not intersecting with each other.
 */
constexpr bool intersects(rectangle const& a, rectangle const& b)
{
	return a.left() < b.right() && b.left() < a.right() && a.top() < b.bottom() && b.top() < a.bottom();
}

}  // namespace sgfx
