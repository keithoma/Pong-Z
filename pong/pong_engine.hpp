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

	/// Resets the game by resetting the ball.
	void reset();

	/// Retrieves the goals each player has made so far.
	points_status points() const noexcept
	{
		return std::make_tuple(goals_left_, goals_right_);
	}

	/// Retrieves maximum number of points a player must get to win.
	unsigned max_points() const noexcept { return max_points_; }

	/// Tests whether or not game is over.
	bool over() const noexcept { return goals_left_ >= max_points_ || goals_right_ >= max_points_; }

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

	unsigned goals_left_;
	unsigned goals_right_;
	std::mt19937 rng_;
	object ball_;
	object left_bat_;
	object right_bat_;
};

}  // namespace pong