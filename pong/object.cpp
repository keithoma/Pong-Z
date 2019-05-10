#include "object.hpp"
#include <sgfx/primitives.hpp>
#include <sgfx/canvas.hpp>

#include <algorithm>
#include <climits>
#include <random>

using namespace std;

namespace pong {

object::object(sgfx::rectangle bounds, sgfx::vec maxVelocities, sgfx::point initialPosition,
			   sgfx::vec initialAcceleration)
	: bounds_{bounds},
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

// @@chris: kann man bestimmt schöner machen
// 		    vielleicht mit abs()
void object::set_velocity(sgfx::vec velocity)
{
	int velox;
	int veloy;

	if (velocity.x < 0) {
		velox = max(velocity.x, - maxVelocities_.x);
	} else {
		velox = min(velocity.x, maxVelocities_.x);
	}

	if (velocity.y < 0) {
		veloy = max(velocity.y, - maxVelocities_.y);
	} else {
		veloy = min(velocity.y, maxVelocities_.y);
	}

	velocity_ = {velox, veloy};
}

void object::reflect_x()
{
	set_velocity({- velocity_.x, velocity_.y});
}

void object::reflect_y()
{
	set_velocity({velocity_.x, - velocity_.y});
}

// @@chris: da diese methode nur für den Ball gebraucht wird, sollte man hier
//          Vererbung benutzen?
void object::reset_game(sgfx::point center)
{
	position_ = center;

	random_device random_device;
	mt19937 random_engine{random_device()};
	uniform_int_distribution<int> distribution{1, 8};

	set_velocity({-6, 6});

	/*
	switch (distribution(random_engine)) {
		// hard velocity
		case 1:
			velocity_ = {3, 4};
			break;
		case 2:
			velocity_ = {3, -4};
			break;
		case 3:
			velocity_ = {-3, 4};
			break;
		case 4:
			velocity_ = {-3, -4};
			break;

		// easy velocity
		case 5:
			velocity_ = {4, 5};
			break;
		case 6:
			velocity_ = {4, -5};
			break;
		case 7:
			velocity_ = {-4, 5};
			break;
		case 8:
			velocity_ = {-4, -5};
			break;
	}
	*/
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

void draw(object const& object, sgfx::canvas const& image, sgfx::widget& target)
{
	sgfx::draw(target, image, object.position());
}

}  // namespace pong
