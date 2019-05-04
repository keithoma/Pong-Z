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
