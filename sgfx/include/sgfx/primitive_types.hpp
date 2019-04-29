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
	int value;
};

struct rectangle {
	point top_left;
	dimension size;
};

}  // namespace sgfx
