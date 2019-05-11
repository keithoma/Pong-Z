#include "pong_engine.hpp"
#include <sgfx/primitive_types.hpp>

#include <algorithm>
#include <array>
#include <random>

#define DEBUG(msg)                       \
	do {                                 \
		fprintf(stderr, "engine: " msg); \
		fprintf(stderr, "\n");           \
	} while (0)

#define DEBUGF(msg, ...)                              \
	do {                                              \
		fprintf(stderr, "engine: " msg, __VA_ARGS__); \
		fprintf(stderr, "\n");                        \
	} while (0)

using namespace std;
using namespace sgfx;

namespace pong {

engine::engine(dimension size, unsigned max_goals)
	: size_{size},
	  max_goals_{max_goals},
	  goals_left_{0},
	  goals_right_{0},
	  rng_{},
	  ball_{
		  {20, 20},                                          // dimension
		  {{10, 10},                                         // boundaries
		   {static_cast<std::uint16_t>(size.width - 40),
			static_cast<std::uint16_t>(size.height - 20)}},  // boundaries
															 // -40 to make it more fluid
		  {6, 6},                                            // max velocity
		  {size.width / 2, size.height / 2},                 // initial pos
		  random_velocity()                                  // initial accel
	  },
	  left_bat_{
		  {20, 100},                         // dimension
		  {{0, 0}, {0, size.height - 100}},  // boundaries
		  {0, 6},                            // max velocities
		  {0, size.height / 2 - 50},         // initial position;
											 // -50 because the bat height is 100
	  },
	  right_bat_{
		  {20, 100},                                                     // dimension
		  {{size.width - 20, 0}, {size.width - 20, size.height - 100}},  // boundaries
		  {0, 6},                                                        // max velocities
		  {size.width - 20, size.height / 2 - 50},                       // initial position;
													// ^ -50 because the bat height is 100
	  }
{
}

/**
 * She accelerates the left bat depending on the given direction.
 *
 * Args:
 *     direction (bat_move): this should be up or down
 */
void engine::move_left_bat(bat_move direction)
{
	if (direction == bat_move::up)
		left_bat_.accelerate({0, -1});
	else
		left_bat_.accelerate({0, +1});
}

/**
 * She stops the left bat by setting the velocity of it to {0, 0}.
 */
void engine::stop_left_bat()
{
	if (left_bat_.velocity() != vec{0, 0})
		left_bat_.set_velocity({0, 0});
}

/**
 * She accelerates the right bat depending on the given direction.
 *
 * Args:
 *     direction (bat_move): this should be up or down
 */
void engine::move_right_bat(bat_move direction)
{
	if (direction == bat_move::up)
		right_bat_.accelerate({0, -1});
	else
		right_bat_.accelerate({0, +1});
}

/**
 * She stops the right bat by setting the velocity of it to {0, 0}.
 */
void engine::stop_right_bat()
{
	if (right_bat_.velocity() != vec{0, 0})
		right_bat_.set_velocity({0, 0});
}

vec engine::random_velocity()
{
	static constexpr array<point, 8> velocities{vec{6, 2}, vec{6, -2}, vec{-6, 2}, vec{-6, -2},
												vec{4, 5}, vec{4, -5}, vec{-4, 5}, vec{-4, -5}};
	return velocities[uniform_int_distribution{0, 7}(rng_)];
}

/**
 * She returns a boolean value for the collision logic. Takes the side of the player as parameter.
 *
 * Args:
 *     side (player): left or right, determines the constants for the bat
 *
 * Returns:
 *     (boolean): true if the ball and the bat collide, false if else
 */
bool engine::collision_logic(player side)
{
	int bat_top;
	int bat_bot;

	// left player; +/-5 so that it feels more fair
	if (side == player::left) {
		bat_top = left_bat_.position().y - 5;
		bat_bot = left_bat_.position().y + 105;

		// right player; +/-5 so that it feels more fair
	}
	else {
		bat_top = right_bat_.position().y - 5;
		bat_bot = right_bat_.position().y + 105;
	}

	// constants for the ball
	const int ball_top = ball_.position().y;
	const int ball_bot = ball_.position().y + 20;

	if (bat_top > ball_top && bat_bot > ball_top || bat_bot < ball_bot && bat_top < ball_bot) {
		return false;
	}
	else {
		return true;
	}
}

void engine::reset()
{
	ball_.set_position(size_ / 2);
	ball_.set_velocity(random_velocity());
}

void engine::update()
{
	DEBUGF("bat %s; ball %s\n", left_bat_.debug_string().c_str(), ball_.debug_string().c_str());

	// TODO: make sure we don't let things move too fast because of faster CPU (by providing own timer)
	left_bat_.update_step();
	right_bat_.update_step();

	switch (ball_.update_step()) {
		case object::status::free:
			break;
		case object::status::stuck_left:
			DEBUG("stuck L");

			if (collision_logic(player::left) == true) {
				ball_.reflect_x();
			}
			else {
				++goals_right_;
				if (goals_right_ >= max_goals_) {
					//@chris
					// print left player won!
				}
				else {
					reset();
				}
			}

			break;
		case object::status::stuck_right:
			DEBUG("stuck R");

			if (collision_logic(player::right) == true) {
				ball_.reflect_x();
			}
			else {
				++goals_left_;
				if (goals_left_ >= max_goals_) {
					//@chris
					// print left player won!
				}
				else {
					reset();
				}
			}

			break;
		case object::status::stuck_top:
		case object::status::stuck_bottom:
			DEBUG("stuck T/B");
			ball_.reflect_y();
			break;

		//@BUG: collision logic need to be implemented here
		case object::status::stuck_top_left:
		case object::status::stuck_bottom_left:
		case object::status::stuck_top_right:
		case object::status::stuck_bottom_right:
			DEBUG("reflect on corner");
			ball_.reflect_x();
			ball_.reflect_y();
		default:
			DEBUG("should never happen");
	}
}

}  // namespace pong
