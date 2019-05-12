#ifndef PONG_OBJECT_H
#define PONG_OBJECT_H

#include <sgfx/primitive_types.hpp>

#include <chrono>
#include <optional>
#include <string>

namespace sgfx {
class widget;
class canvas;
}  // namespace sgfx

namespace pong {

class object {
  public:
	enum class status {
		free,
		stuck_left,
		stuck_right,
		stuck_top,
		stuck_bottom,
		stuck_top_left,
		stuck_bottom_left,
		stuck_top_right,
		stuck_bottom_right
	};

	/**
	 * Constructs an object.
	 *
	 * @p size object dimension
	 * @p bounds the bounds within this object may be
	 * @p maxVelocities maximum velocity per coordinate this object may gain
	 * @p initialPosition the objects initial position
	 */
	constexpr object(sgfx::dimension size, sgfx::rectangle bounds, sgfx::vec maxVelocities,
					 sgfx::point initialPosition, sgfx::vec initialAcceleration,
					 bool decelerating)
		: size_{size},
		  bounds_{bounds},
		  maxVelocities_{maxVelocities},
		  position_{initialPosition},
		  velocity_{initialAcceleration},
		  acceleration_{0, 0},
		  status_{status::free},
		  decelerating_{decelerating}
	{
	}

	/// sets the object to the desired position
	void set_position(sgfx::point position);

	/// Accelerates the object by increasing its velocity by given @p acceleration.
	void accelerate(sgfx::vec acceleration);

	/// Sets the objects velocity to a fixed value @p velocity.
	void set_velocity(sgfx::vec vecocity);

	/// Reflects the ball at its current X position (horizontal wall)
	void reflect_x();

	/// Reflects the ball at its current Y position (vertical wall)
	void reflect_y();

	/// Updates its current position with regard to acceleration & bounds
	status update(std::chrono::duration<double> delta);

	/// Retrieves the object dimensions
	sgfx::dimension const& size() const noexcept { return size_; }

	/// Retrieves the current object position (top-left)
	sgfx::point const& position() const noexcept { return position_; }

	/// Retrieves the current object velocity (speed).
	sgfx::vec const& velocity() const noexcept { return velocity_; }

	/// Retrieves the geometry (bounding box) of this object in space.
	sgfx::rectangle bounding_box() const noexcept { return sgfx::rectangle{position_, size_}; }

	std::string debug_string() const
	{
		char buf[128];
		auto n = snprintf(buf, sizeof(buf), "pos=(%d, %d), velocity=(%d, %d)", position_.x, position_.y,
						  velocity_.x, velocity_.y);
		return std::string(buf, n);
	}

  private:
	const sgfx::dimension size_;
	const sgfx::rectangle bounds_;
	const sgfx::vec maxVelocities_;
	sgfx::point position_;
	sgfx::vec velocity_;
	sgfx::vec acceleration_;
	status status_;
	bool decelerating_;
};

constexpr bool is_colliding(object const& a, object const& b) noexcept
{
	return intersects(a.bounding_box(), b.bounding_box());
}

void draw(object const& object, sgfx::canvas const& image, sgfx::widget& target);

}  // namespace pong

#endif
