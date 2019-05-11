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

std::ostream& operator<<(std::ostream& os, player p)
{
	if (p == player::left)
		os << "left";
	else
		os << "right";
	return os;
}
engine::engine(dimension size, unsigned max_goals, player_callback goal, player_callback game_won)
	: size_{size},
	  max_goals_{max_goals},
	  goal_{move(goal)},
	  game_won_{move(game_won)},
	  goals_left_{0},
	  goals_right_{0},
	  rng_{},
	  ball_{
		  {20, 20},   // dimension
		  {{10, 10},  // boundaries
		   {static_cast<std::uint16_t>(size.width - 40),
			static_cast<std::uint16_t>(size.height - 20)}},  // boundaries
															 // -40 to make it more fluid
		  {6, 6},                                            // max velocity
		  {size.width / 2, size.height / 2},                 // initial pos
		  {6, 0}  // random_velocity()                           // initial accel
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

void engine::reset()
{
	ball_.set_position(size_ / 2);
	ball_.set_velocity(random_velocity());
}

void engine::update()
{
	// TODO: make sure we don't let things move too fast because of faster CPU (by providing own timer)
	left_bat_.update_step();
	right_bat_.update_step();

	switch (ball_.update_step()) {
		case object::status::free:
			break;
		case object::status::stuck_left:
			DEBUG("stuck L");

			if (is_colliding(ball_, left_bat_))
				ball_.reflect_x();
			else {
				++goals_right_;
				if (goals_right_ >= max_goals_) {
					game_won_(player::left);
				}
				else {
					reset();
				}
			}

			break;
		case object::status::stuck_right:
			DEBUG("stuck R");

			if (is_colliding(ball_, right_bat_))
				ball_.reflect_x();
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
