#include "pong_engine.hpp"

#include <vector>
#include <algorithm>
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

namespace pong {

engine::engine(int sizeX, int sizeY, unsigned max_goals)
	: sizeX_{sizeX},
	  sizeY_{sizeY},
	  max_goals_{max_goals},
	  goals_left_{0},
	  goals_right_{0},
	  ball_{
		  {{10, 10},
		   {static_cast<std::uint16_t>(sizeX - 20), static_cast<std::uint16_t>(sizeY - 20)}},  // boundaries
		  {6, 6},                                                                              // max velocity
		  {sizeX / 2, sizeY / 2},                                                              // initial pos
		  {-6, 0}  // initial accel
	  },
	  left_bat_{
		  {{0, 0}, {0, sizeY - 100}},  // boundaries
		  {0, 6},                      // max velocities
		  {0, sizeY / 2 - 50},         // initial position;
									   // -50 because the bat height is 100
	  },
	  right_bat_{
		  {{sizeX - 20, 0}, {sizeX - 20, sizeY - 100}},  // boundaries
		  {0, 6},                                        // max velocities
		  {sizeX - 20, sizeY / 2 - 50},                  // initial position;
														 // ^ -50 because the bat height is 100
	  }
{
}

// it should only accelerate if the bats are not on their boundaries
// or alternatively, set acceration to 0 if the bats are on their boundaries

void engine::move_left_bat(bat_move direction)
{
	if (direction == bat_move::up)
		left_bat_.accelerate({0, -1});
	else
		left_bat_.accelerate({0, +1});
}

void engine::move_right_bat(bat_move direction)
{
	if (direction == bat_move::up)
		right_bat_.accelerate({0, -1});
	else
		right_bat_.accelerate({0, +1});
}


// @@chris kann man bestimmt mit einem pointer in eine funktion zusammenfassen

bool engine::collision_logic_left()
{
	const int bat_top = left_bat_.position().y;
	const int bat_bot = left_bat_.position().y + 100;
	const int ball_top = ball_.position().y;
	const int ball_bot = ball_.position().y + 20;

	if (bat_top > ball_top && bat_bot > ball_top ||
	    bat_bot < ball_bot && bat_top < ball_bot) {
		return false;
	} else {
		return true;
	}
}

bool engine::collision_logic_right()
{
	const int bat_top = right_bat_.position().y;
	const int bat_bot = right_bat_.position().y + 100;
	const int ball_top = ball_.position().y;
	const int ball_bot = ball_.position().y + 20;

	if (bat_top > ball_top && bat_bot > ball_top ||
	    bat_bot < ball_bot && bat_top < ball_bot) {
		return false;
	} else {
		return true;
	}
}


// @@chris: da diese methode nur für den Ball gebraucht wird, sollte man hier
//          Vererbung benutzen?
void engine::reset_game(sgfx::point center)
{
	ball_.set_position(center);

	random_device random_device;
	mt19937 random_engine{random_device()};
	uniform_int_distribution<int> distribution{1, 8};

	switch (distribution(random_engine)) {
		// easy velocity
		case 1:
			ball_.set_velocity({6, 2});
			break;
		case 2:
			ball_.set_velocity({6, -2});
			break;
		case 3:
			ball_.set_velocity({-6, 2});
			break;
		case 4:
			ball_.set_velocity({-6, -2});
			break;

		// hard velocity
		case 5:
			ball_.set_velocity({4, 5});
			break;
		case 6:
			ball_.set_velocity({4, -5});
			break;
		case 7:
			ball_.set_velocity({-4, 5});
			break;
		case 8:
			ball_.set_velocity({-4, -5});
			break;
	}
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

			if (collision_logic_left() == true) {
				ball_.reflect_x();
			} else {
				++goals_right_;
				if (goals_right_ >= max_goals_) {
					//@chris
					//print left player won!
				} else {
					reset_game({sizeX_ / 2, sizeY_ / 2});
				}
			}
			
			break;
		case object::status::stuck_right:
			DEBUG("stuck R");

			if (collision_logic_right() == true) {
				ball_.reflect_x();
			} else {
				++goals_left_;
				if (goals_left_ >= max_goals_) {
					//@chris
					//print left player won!
				} else {
					reset_game({sizeX_ / 2, sizeY_ / 2});
				}
			}

			break;
		case object::status::stuck_top:
		case object::status::stuck_bottom:
			DEBUG("stuck T/B");
			ball_.reflect_y();
			break;
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
