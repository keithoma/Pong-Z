#pragma once

#include "object.hpp"

#include <tuple>

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

/**
 * Pong Game Engine.
 */
class engine {
  public:
	engine(int sizeX, int sizeY, unsigned max_goals);

	// Moves the left bat into the given direction.
	void move_left_bat(bat_move direction);

	// Moves the left bat into the given direction.
	void move_right_bat(bat_move direction);

	// Updates the world state.
	void update();

	void reset_game(sgfx::point center);
	
	// Retrieves the goals each player has made so far.
	std::tuple<unsigned, unsigned> points() const noexcept { return std::make_tuple(goals_left_, goals_right_); }

	// Object accessors (use them for drawing those objects with a canvas to a window).
	object const& left_bat() const noexcept { return left_bat_; }
	object const& right_bat() const noexcept { return right_bat_; }
	object const& ball() const noexcept { return ball_; }

  private:
  	int sizeX_;
	int sizeY_;
	unsigned max_goals_;
	unsigned goals_left_;
	unsigned goals_right_;
	object ball_;
	object left_bat_;
	object right_bat_;

	bool collision_logic_left();
	bool collision_logic_right();

};

}  // namespace pong