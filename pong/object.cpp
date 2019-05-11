#include "object.hpp"
#include <sgfx/canvas.hpp>
#include <sgfx/primitives.hpp>

#include <algorithm>
#include <climits>

using namespace std;
using namespace sgfx;

namespace pong {

object::object(dimension size, rectangle bounds, vec maxVelocities, point initialPosition,
			   vec initialAcceleration)
	: size_{size},
	  bounds_{bounds},
	  maxVelocities_{maxVelocities},
	  position_{initialPosition},
	  velocity_{initialAcceleration},
	  status_{status::free}
{
}

void object::set_position(point position)
{
	position_ = {position};
}

void object::accelerate(vec acceleration)
{
	set_velocity(velocity_ + acceleration);
}

// @@chris: kann man bestimmt schöner machen
// 		    vielleicht mit abs()
void object::set_velocity(vec velocity)
{
	int velox;
	int veloy;

	if (velocity.x < 0) {
		velox = max(velocity.x, -maxVelocities_.x);
	}
	else {
		velox = min(velocity.x, maxVelocities_.x);
	}

	if (velocity.y < 0) {
		veloy = max(velocity.y, -maxVelocities_.y);
	}
	else {
		veloy = min(velocity.y, maxVelocities_.y);
	}

	velocity_ = {velox, veloy};
}

void object::reflect_x()
{
	set_velocity({-velocity_.x, velocity_.y});
}

void object::reflect_y()
{
	set_velocity({velocity_.x, -velocity_.y});
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
	if (position_.x == bounds_.top_left.x && position_.y == bounds_.top_left.y) {
		status_ = status::stuck_top_left;
	}
	else if (position_.x == bounds_.top_left.x && position_.y == bounds_.top_left.y + bounds_.size.height) {
		status_ = status::stuck_bottom_left;
	}
	else if (position_.x == bounds_.top_left.x + bounds_.size.width && position_.y == bounds_.top_left.y) {
		status_ = status::stuck_top_right;
	}
	else if (position_.x == bounds_.top_left.x + bounds_.size.width
			 && position_.y == bounds_.top_left.y + bounds_.size.height) {
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

void draw(object const& object, canvas const& image, widget& target)
{
	sgfx::draw(target, image, object.position());
}

}  // namespace pong
