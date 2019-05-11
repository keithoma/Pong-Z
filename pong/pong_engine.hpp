#pragma once

#include <sgfx/primitive_types.hpp>
#include "object.hpp"

#include <functional>
#include <random>
#include <tuple>
#include <iosfwd>

namespace pong {

enum class bat_move {
	up,
	down,
};

// not used right now
enum class player {
	left,
	right,
};

std::ostream& operator<<(std::ostream& os, player p);

/**
 * Pong Game Engine.
 */
class engine {
  public:
	using player_callback = std::function<void(player)>;

	engine(sgfx::dimension size, unsigned max_goals, player_callback on_goal, player_callback on_game_won);

	// Moves the left bat into the given direction.
	void move_left_bat(bat_move direction);

	// stops the left bat
	void stop_left_bat();

	// Moves the left bat into the given direction.
	void move_right_bat(bat_move direction);

	// stops the left bat
	void stop_right_bat();

	// Updates the world state.
	void update();

	// Resets the game by resetting the ball.
	void reset();

	// Retrieves the goals each player has made so far.
	std::tuple<unsigned, unsigned> points() const noexcept
	{
		return std::make_tuple(goals_left_, goals_right_);
	}

	// Object accessors (use them for drawing those objects with a canvas to a window).
	object const& left_bat() const noexcept { return left_bat_; }
	object const& right_bat() const noexcept { return right_bat_; }
	object const& ball() const noexcept { return ball_; }

  private:
	/// Constructs a random velocity.
	sgfx::vec random_velocity();

  private:
	sgfx::dimension size_;
	unsigned max_goals_;
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