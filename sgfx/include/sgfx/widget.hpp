#pragma once

#include <sgfx/primitive_types.hpp>

#include <cstdint>
#include <vector>

namespace sgfx {

/**
 * widget provides an abstract interface to canvas and window.
 */
class widget {
  public:
	virtual ~widget() = default;

	virtual std::uint16_t width() const noexcept = 0;
	virtual std::uint16_t height() const noexcept = 0;

	virtual std::vector<color::rgb_color>& pixels() noexcept = 0;
	virtual const std::vector<color::rgb_color>& pixels() const noexcept = 0;

	dimension size() const noexcept { return {width(), height()}; }

	color::rgb_color& operator[](point const& p) { return pixels()[p.y * width() + p.x]; }
	color::rgb_color const& operator[](point const& p) const { return pixels()[p.y * width() + p.x]; }
};

}  // namespace sgfx
