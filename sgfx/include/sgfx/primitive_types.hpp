#pragma once

#include <cstdint>

namespace sgfx {

struct vec {
};

struct point {
	int x;
	int y;
};

struct dimension {
	// [Kei]: I think this should be right
	int width;
	int height;
};

struct rectangle {
	point top_left;
	dimension size;
};

}  // namespace sgfx
