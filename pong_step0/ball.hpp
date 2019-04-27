#ifndef PONG_BALL_H
#define PONG_BALL_H

#include <sgfx/canvas.hpp>
#include <sgfx/primitive_types.hpp>

#include <optional>

namespace pong
{
	class ball
	{
		public:
		enum class status
		{
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
		
		ball();
		
		void move_to(sgfx::point position);
		void bind_to(sgfx::rectangle bounds);
		void accelerate(sgfx::vec acceleration);
		void limit_velocity(sgfx::vec max_velocities);
		void set_velocity(sgfx::vec vecocity);
		
		void reflect_x();
		void reflect_y();
		
		status update_step();
		void draw(sgfx::canvas_or_window_or_something target) const;
		
	};
}

#endif
