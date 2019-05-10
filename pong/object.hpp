#ifndef PONG_OBJECT_H
#define PONG_OBJECT_H

#include <sgfx/canvas.hpp>
#include <sgfx/primitive_types.hpp>

#include <optional>
#include <string>

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
	 * @p image the visual to use when drawing this object
	 * @p bounds the bounds within this object may be
	 * @p maxVelocities maximum velocity per coordinate this object may gain
	 * @p initialPosition the objects initial position
	 */
	object(sgfx::canvas image, sgfx::rectangle bounds, sgfx::vec maxVelocities, sgfx::point initialPosition,
		   sgfx::vec initialAcceleration = {0, 0});

	/// Accelerates the object by increasing its velocity by given @p acceleration.
	void accelerate(sgfx::vec acceleration);

	/// Sets the objects velocity to a fixed value @p velocity.
	void set_velocity(sgfx::vec vecocity);

	/// Reflects the ball at its current X position (horizontal wall)
	void reflect_x();

	/// Reflects the ball at its current Y position (vertical wall)
	void reflect_y();

	/// resets the game by moving the ball to the center and asigning a random preset velocity
	void reset_game(sgfx::point center);

	/// Updates its current position with regard to acceleration & bounds
	status update_step();

	void draw(sgfx::widget& target) const;

	sgfx::point const& position() const noexcept { return position_; }
	sgfx::vec const& velocity() const noexcept { return velocity_; }

	std::string debug_string() const
	{
		char buf[128];
		auto n = snprintf(buf, sizeof(buf), "pos=(%d, %d), velocity=(%d, %d)", position_.x, position_.y,
						  velocity_.x, velocity_.y);
		return std::string(buf, n);
	}

  private:
	const sgfx::canvas image_;
	const sgfx::rectangle bounds_;
	const sgfx::vec maxVelocities_;
	sgfx::point position_;
	sgfx::vec velocity_;
	status status_;
};

}  // namespace pong

#endif
