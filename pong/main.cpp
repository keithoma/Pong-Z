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

	auto engine = pong::engine{1024, 768, 8};

	// TODO: ball.reset_game({main_window.width() / 2, main_window.height() / 2});

	while (main_window.handle_events() && !main_window.should_close()) {
		engine.update();

		if (main_window.is_pressed(key::escape))
			break;

		if (main_window.is_pressed(key::wkey))
			engine.move_left_bat(pong::bat_move::up);

		if (main_window.is_pressed(key::skey))
			engine.move_left_bat(pong::bat_move::down);

		if (main_window.is_pressed(key::up))
			engine.move_right_bat(pong::bat_move::up);

		if (main_window.is_pressed(key::down))
			engine.move_right_bat(pong::bat_move::down);

		clear(main_window, color::black);
		pong::draw(engine.left_bat(), bat_img, main_window);
		pong::draw(engine.right_bat(), bat_img, main_window);
		pong::draw(engine.ball(), ball_img, main_window);

		main_window.show();
	};

	return 0;
}
