#ifndef PONG_OBJECT_H
#define PONG_OBJECT_H

#include <sgfx/canvas.hpp>
#include <sgfx/primitive_types.hpp>

#include <optional>

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
	 * @p initialPosition the objects initial position 
	 */
	object(
		sgfx::canvas image,
		sgfx::rectangle bounds,
		sgfx::point initialPosition,
		sgfx::vec maxVelocities,
		sgfx::vec initialAcceleration = {0, 0}
	);

	/// Accelerates the object into the given direction (bigger values = faster)
	void accelerate(sgfx::vec acceleration);

	void limit_velocity(sgfx::vec max_velocities);
	void set_velocity(sgfx::vec vecocity);

	/// Reflects the ball at its current X position (horizontal wall)
	void reflect_x();

	/// Reflects the ball at its current Y position (vertical wall)
	void reflect_y();

	status update_step();

	void draw(sgfx::widget& target) const;

  private:
	sgfx::canvas image_;
	sgfx::rectangle bounds_;
	sgfx::point currentPosition_;
	sgfx::vec acceleration_;
};

}  // namespace pong

#endif
