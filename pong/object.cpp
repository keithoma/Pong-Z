#include "object.hpp"
#include <sgfx/primitives.hpp>

#include <algorithm>
#include <climits>

using namespace std;

namespace pong {

object::object(sgfx::canvas image, sgfx::rectangle bounds, sgfx::vec maxVelocities,
			   sgfx::point initialPosition, sgfx::vec initialAcceleration)
	: image_{image},
	  bounds_{bounds},
	  maxVelocities_{maxVelocities},
	  position_{initialPosition},
	  velocity_{initialAcceleration},
	  status_{status::free}
{
}

void object::accelerate(sgfx::vec acceleration)
{
	set_velocity(velocity_ + acceleration);
}

void object::set_velocity(sgfx::vec velocity)
{
	velocity_ = {min(velocity.x, maxVelocities_.x), min(velocity.y, maxVelocities_.y)};
}

void object::reflect_x()
{
	set_velocity({- velocity_.x, velocity_.y});
}

void object::reflect_y()
{
	set_velocity({velocity_.x, - velocity_.y});
}

object::status object::update_step()
{
	static auto const clamp = [](int low, int val, int high) {
		if (val < low)
			return low;
		else if (val > high)
			return high;
		else
			return val;
	};

	position_.x =
		clamp(bounds_.top_left.x, position_.x + velocity_.x, bounds_.top_left.x + bounds_.size.width);

	position_.y =
		clamp(bounds_.top_left.y, position_.y + velocity_.y, bounds_.top_left.y + bounds_.size.height);

	// edge cases; sets 'status_' to 'stuck_top_left', 'stuck_bottom_left', 'stuck_top_right' or
	// 'stuck_bottom_right'
	if (position_.x == bounds_.top_left.x && 
	    position_.y == bounds_.top_left.y) {
		status_ = status::stuck_top_left;
	}
	else if (position_.x == bounds_.top_left.x && 
	         position_.y == bounds_.top_left.y + bounds_.size.height) {
		status_ = status::stuck_bottom_left;
	}
	else if (position_.x == bounds_.top_left.x + bounds_.size.width && 
	         position_.y == bounds_.top_left.y) {
		status_ = status::stuck_top_right;
	}
	else if (position_.x == bounds_.top_left.x + bounds_.size.width && 
	         position_.y == bounds_.top_left.y + bounds_.size.height) {
		status_ = status::stuck_bottom_right;
	}

	// side cases; sets 'status_' to 'stuck_left', 'stuck_right', 'stuck_top' or 'stuck_bottom'
	else if (position_.x == bounds_.top_left.x) {
		status_ = status::stuck_left;
	}
	else if (position_.x == bounds_.top_left.x + bounds_.size.width) {
		status_ = status::stuck_right;
	}
	else if (position_.y == bounds_.top_left.y) {
		status_ = status::stuck_top;
	}
	else if (position_.y == bounds_.top_left.y + bounds_.size.height) {
		status_ = status::stuck_bottom;
	} 
	
	// in all other cases, which therefore means the ball is not stuck, set 'status_' to 'free'
	else {
		status_ = status::free;
	}
	return status_;
}

void object::draw(sgfx::widget& target) const
{
	sgfx::draw(target, image_, position_);
}

}  // namespace pong
