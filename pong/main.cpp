#include "object.hpp"

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <cassert>
#include <cstdio>

using namespace sgfx;
using namespace pong;
using namespace std;

#define DEBUG(msg) \
	do {                                   \
		fprintf(stderr, msg);              \
		fprintf(stderr, "\n");             \
	} while (0)

#define DEBUGF(msg, ...)                   \
	do {                                   \
		fprintf(stderr, msg, __VA_ARGS__); \
		fprintf(stderr, "\n");             \
	} while (0)

int main(int argc, char* argv[])
{
	auto main_window = window{1024, 768};

	// create visual for ball:
	auto const ball_img = canvas::colored({20, 20}, color::red);

	// create visual for bat:
	auto const bat_img = canvas::colored({20, 100}, color::blue);

	// create object for ball using its visual from above
	auto ball = object{
		ball_img,
		rectangle{{10, 10},
				  {static_cast<std::uint16_t>(main_window.width() - 20),
				   static_cast<std::uint16_t>(main_window.height() - 20)}}, // boundaries
		{6, 6},																// max velocity
		{main_window.width() / 2, main_window.height() / 2},                // initial pos
		{1, 2}                                                              // initial accel
	};
	ball.reset_game({main_window.width() / 2, main_window.height() / 2});

	// create object for bat using its visual from above
	// max velocity should be bounded in to negative values as well
	auto bat_left = object{
		bat_img,
		{{0, 0}, {0, unsigned(main_window.height() - 100)}}, // boundaries
		{0, 6},                                               // max velocities
		{0, main_window.height() / 2 - 50},                   // initial position;
		                                                      // -50 because the bat height is 100
	};

	auto bat_right = object{
		bat_img,
		{{unsigned(main_window.width() - 20), 0}, 
		 {unsigned(main_window.width() - 20), unsigned(main_window.height() - 100)}}, // boundaries
		{0, 6},                                                             // max velocities
		{main_window.width() - 20, main_window.height() / 2 - 50},          // initial position;
		                                                        // ^ -50 because the bat height is 100
	};

	while (main_window.handle_events() && !main_window.should_close()) {
		DEBUGF("bat %s; ball %s\n", bat_left.debug_string().c_str(), ball.debug_string().c_str());
		switch (ball.update_step()) {
			case object::status::free:
				break;
			case object::status::stuck_left:
				DEBUG("stuck L");
				ball.reflect_x();
				break;
			case object::status::stuck_right:
				DEBUG("stuck R");
				ball.reflect_x();
				break;
			case object::status::stuck_top:
			case object::status::stuck_bottom:
				DEBUG("stuck T/B");
				ball.reflect_y();
				break;
			case object::status::stuck_top_left:
			case object::status::stuck_bottom_left:
			case object::status::stuck_top_right:
			case object::status::stuck_bottom_right:
				DEBUG("reflect on corner");
				ball.reflect_x();
				ball.reflect_y();
			default:
				DEBUG("should never happen");
		}

		bat_left.update_step();
		bat_right.update_step();

		if (main_window.is_pressed(key::escape))
			break;


		// it should only accelerate if the bats are not on their boundaries
		// or alternatively, set acceration to 0 if the bats are on their boundaries
		if (main_window.is_pressed(key::wkey)) {
			DEBUG("KBD WKey");
			bat_left.accelerate({0, -1});
		}

		if (main_window.is_pressed(key::skey)) {
			DEBUG("KBD SKey");
			bat_left.accelerate({0, 1});
		}

		if (main_window.is_pressed(key::up)) {
			DEBUG("KBD Up");
			bat_right.accelerate({0, -1});
		}

		if (main_window.is_pressed(key::down)) {
			DEBUG("KBD Down");
			bat_right.accelerate({0, 1});
		}

		clear(main_window, color::black);
		bat_left.draw(main_window);
		bat_right.draw(main_window);
		ball.draw(main_window);

		main_window.show();
	};

	return 0;
}
