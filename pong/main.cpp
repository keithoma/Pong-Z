#include "object.hpp"
#include "pong_engine.hpp"

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/canvas.hpp>
#include <sgfx/window.hpp>

#include <iostream>
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

	// some game feedback implementations
	auto on_goal = [](player player) { cout << "Player " << player << " has scored a point." << endl; };
	auto on_game_won = [](player player) { cout << "Player " << player << " has won this match!" << endl; };

	// initialize the engine; parameters are: window width, window height, goals to win
	auto game = pong::engine{dimension{1024, 768}, 8, on_goal, on_game_won};

	// create visual for ball
	auto const ball_img = canvas::colored(game.ball().size(), color::red);

	// create visual for bat (both bats have same size)
	auto const bat_img = canvas::colored(game.left_bat().size(), color::blue);

	while (main_window.handle_events() && !main_window.should_close()) {
		game.update();

		if (game.over() || main_window.is_pressed(key::escape))
			break;

		// looks cryptic, but essentially moves the bat according to the keys that are pressed
		// if no keys are pressed, the bat stops
		//
		// left bat:
		if (main_window.is_pressed(key::wkey) &&
		   (!main_window.is_pressed(key::skey))) {
			game.move_left_bat(pong::bat_move::up);
		} else if (main_window.is_pressed(key::skey) &&
		          (!main_window.is_pressed(key::wkey))) {
			game.move_left_bat(pong::bat_move::down);
		} else {
			game.stop_left_bat();
		}

		// right bat:
		if (main_window.is_pressed(key::up) &&
		   (!main_window.is_pressed(key::down))) {
			game.move_right_bat(pong::bat_move::up);
		} else if (main_window.is_pressed(key::down) &&
		          (!main_window.is_pressed(key::up))) {
			game.move_right_bat(pong::bat_move::down);
		} else {
			game.stop_right_bat();
		}

		clear(main_window, color::black);
		pong::draw(game.left_bat(), bat_img, main_window);
		pong::draw(game.right_bat(), bat_img, main_window);
		pong::draw(game.ball(), ball_img, main_window);

		main_window.show();
	};

	return 0;
}
