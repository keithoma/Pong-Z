#include "object.hpp"
#include "pong_engine.hpp"

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/canvas.hpp>
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

	// initialize the engine; parameters are: window width, window height, goals to win
	auto engine = pong::engine{dimension{1024, 768}, 8};

	while (main_window.handle_events() && !main_window.should_close()) {
		engine.update();

		if (main_window.is_pressed(key::escape))
			break;

		// looks cryptic, but essentially moves the bat according to the keys that are pressed
		// if no keys are pressed, the bat stops
		//
		// left bat:
		if (main_window.is_pressed(key::wkey) &&
		   (!main_window.is_pressed(key::skey))) {
			engine.move_left_bat(pong::bat_move::up);
		} else if (main_window.is_pressed(key::skey) &&
		          (!main_window.is_pressed(key::wkey))) {
			engine.move_left_bat(pong::bat_move::down);
		} else {
			engine.stop_left_bat();
		}

		// right bat:
		if (main_window.is_pressed(key::up) &&
		   (!main_window.is_pressed(key::down))) {
			engine.move_right_bat(pong::bat_move::up);
		} else if (main_window.is_pressed(key::down) &&
		          (!main_window.is_pressed(key::up))) {
			engine.move_right_bat(pong::bat_move::down);
		} else {
			engine.stop_right_bat();
		}

		clear(main_window, color::black);
		pong::draw(engine.left_bat(), bat_img, main_window);
		pong::draw(engine.right_bat(), bat_img, main_window);
		pong::draw(engine.ball(), ball_img, main_window);

		main_window.show();
	};

	return 0;
}
