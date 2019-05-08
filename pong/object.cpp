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
	// TODO impl
}

void object::reflect_y()
{
	// TODO impl
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

	// TODO: update status_
	return status_;
}

void object::draw(sgfx::widget& target) const
{
	sgfx::draw(target, image_, position_);
}

}  // namespace pong
