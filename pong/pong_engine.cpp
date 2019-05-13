// This file is part of the "pong" project, http://github.com/keithoma/pong>
//   (c) 2019-2019 Christian Parpart <christian@parpart.family>
//   (c) 2019-2019 Kei Thoma <thomakmj@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include "pong_engine.hpp"
#include <sgfx/primitive_types.hpp>

#include <algorithm>
#include <array>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;
using namespace sgfx;

namespace pong {

engine::engine(dimension size, unsigned max_goals, player_callback goal, player_callback game_won)
	: size_{size},
	  max_points_{max_goals},
	  goal_{move(goal)},
	  game_won_{move(game_won)},
	  points_left_{0},
	  points_right_{0},
	  rng_{},
	  ball_{
		  {20, 20},   // dimension
		  {{10, 10},  // boundaries
		   {static_cast<uint16_t>(size.width - 40),
			static_cast<uint16_t>(size.height - 20)}},  // boundaries
														// -40 to make it more fluid
		  {6, 6},                                       // max velocity
		  size / 2,                                     // initial pos
		  random_velocity(),                            // initial accel
		  false,
	  },
	  left_bat_{
		  {20, 100},                         // dimension
		  {{0, 0}, {0, size.height - 100}},  // boundaries
		  {0, 6},                            // max velocities
		  {0, size.height / 2 - 50},         // initial position;
											 // -50 because the bat height is 100
		  {0, 0},                            // initial accel
		  true,
	  },
	  right_bat_{
		  {20, 100},                                                     // dimension
		  {{size.width - 20, 0}, {size.width - 20, size.height - 100}},  // boundaries
		  {0, 6},                                                        // max velocities
		  {size.width - 20, size.height / 2 - 50},  // initial position (-50 because the bat height is 100)
		  {0, 0},                                   // initial accel
		  true,
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
	static constexpr array<int, 2> ys = {-1, +1};
	left_bat_.accelerate({0, 5 * ys.at(static_cast<size_t>(direction))});
}

/**
 * She accelerates the right bat depending on the given direction.
 *
 * Args:
 *     direction (bat_move): this should be up or down
 */
void engine::move_right_bat(bat_move direction)
{
	static constexpr array<int, 2> ys = {-1, +1};
	right_bat_.accelerate({0, 5 * ys.at(static_cast<size_t>(direction))});
}

vec<int> engine::random_velocity()
{
	using V = vec<int>;
	static constexpr array<point, 8> velocities{V{6, 2}, V{6, -2}, V{-6, 2}, V{-6, -2},
												V{4, 5}, V{4, -5}, V{-4, 5}, V{-4, -5}};
	return velocities[uniform_int_distribution{0, 7}(rng_)];
}

void engine::freeze()
{
	ball_.set_velocity({0, 0});
}

void engine::reset_ball()
{
	ball_.set_position(size_ / 2);
	ball_.set_velocity(random_velocity());
}

void engine::reset()
{
	reset_ball();

	points_left_ = 0;
	points_right_ = 0;
}

void engine::update(duration<double> delta)
{
	left_bat_.update(delta);
	right_bat_.update(delta);

	switch (ball_.update(delta)) {
		case object::status::free:
			if (is_colliding(ball_, left_bat_))
				ball_.reflect_x();
			if (is_colliding(ball_, right_bat_))
				ball_.reflect_x();
			break;
		case object::status::stuck_left:
			if (is_colliding(ball_, left_bat_))
				ball_.reflect_x();
			else {
				++points_right_;
				goal_(player::right, points());
				if (points_right_ < max_points_)
					reset_ball();
				else
					game_won_(player::right, points());
			}
			break;
		case object::status::stuck_right:
			if (is_colliding(ball_, right_bat_))
				ball_.reflect_x();
			else {
				++points_left_;
				goal_(player::left, points());
				if (points_left_ < max_points_)
					reset_ball();
				else
					game_won_(player::left, points());
			}
			break;
		case object::status::stuck_top:
		case object::status::stuck_bottom:
			ball_.reflect_y();
			break;

		// edge cases; perhaps redundant
		case object::status::stuck_top_left:
		case object::status::stuck_bottom_left:
			if (is_colliding(ball_, left_bat_)) {
				ball_.reflect_x();
				ball_.reflect_y();
			}
			else {
				++points_right_;
				goal_(player::right, points());
				if (points_right_ < max_points_)
					reset_ball();
				else
					game_won_(player::right, points());
			}
			break;
		case object::status::stuck_top_right:
		case object::status::stuck_bottom_right:
			if (is_colliding(ball_, right_bat_)) {
				ball_.reflect_x();
				ball_.reflect_y();
			}
			else {
				++points_left_;
				goal_(player::left, points());
				if (points_left_ < max_points_)
					reset_ball();
				else
					game_won_(player::left, points());
			}
			break;
		default:
			// should never happen
			break;
	}
}

}  // namespace pong
