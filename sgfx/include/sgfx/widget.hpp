#pragma once
#include <sgfx/primitive_types.hpp>
#include <vector>
#include <cstdint>

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
};

}  // namespace sgfx
