// This file is part of the "pong" project, http://github.com/keithoma/pong>
//   (c) 2019-2019 Christian Parpart <christian@parpart.family>
//   (c) 2019-2019 Kei Thoma <thomakmj@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include "object.hpp"

#include <sgfx/primitive_types.hpp>

#include <chrono>
#include <cstddef>
#include <functional>
#include <random>
#include <tuple>
#include <iosfwd>

namespace pong {

enum class bat_move : std::size_t {
	up = 0,
	down = 1,
};

enum class player : std::size_t {
	left = 0,
	right = 1,
};

/**
 * Pong Game Engine.
 */
class engine {
  public:
	using points_status = std::tuple<unsigned, unsigned>;
	using player_callback = std::function<void(player, points_status)>;

	/**
	  * Constructs the game engine.
	  *
	  * @p size        Dimensions for internal coordinate system.
	  * @p max_goals   Goal count needed to declare one player a winnner.
	  * @p on_goal     Callback to be invoked when a goal occured.
	  * @p on_game_won Callback to be invoked upon last goal to declare a winner.
	  */
	engine(sgfx::dimension size, unsigned max_goals, player_callback on_goal, player_callback on_game_won);

	/// Moves the left bat into the given direction.
	void move_left_bat(bat_move direction);

	/// Moves the left bat into the given direction.
	void move_right_bat(bat_move direction);

	/// Updates the world state.
	void update(std::chrono::duration<double> delta);

	/// Freezes the game (for example right at the end of a game, to see the scores, ...)
	void freeze();

	/// Resets the game by resetting internal state (ball and points).
	void reset();

	/// Resets the ball and only the ball.
	void reset_ball();

	/// Retrieves the goals each player has made so far.
	points_status points() const noexcept
	{
		return std::make_tuple(points_left_, points_right_);
	}

	/// Retrieves maximum number of points a player must get to win.
	unsigned max_points() const noexcept { return max_points_; }

	/// Tests whether or not game is over.
	bool over() const noexcept { return points_left_ >= max_points_ || points_right_ >= max_points_; }

	// Object accessors (use them for drawing those objects with a canvas to a window).
	object const& left_bat() const noexcept { return left_bat_; }
	object const& right_bat() const noexcept { return right_bat_; }
	object const& ball() const noexcept { return ball_; }

  private:
	/// Constructs a random velocity.
	sgfx::vec random_velocity();

  private:
	sgfx::dimension size_;
	unsigned max_points_;
	player_callback goal_;
	player_callback game_won_;

	unsigned points_left_;
	unsigned points_right_;
	std::mt19937 rng_;
	object ball_;
	object left_bat_;
	object right_bat_;
};

}  // namespace pong
