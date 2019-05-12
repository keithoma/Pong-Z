#include "object.hpp"
#include "pong_engine.hpp"

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <cassert>
#include <chrono>
#include <cstdio>
#include <iostream>

using namespace sgfx;
using namespace pong;
using namespace std;

#define DEBUG(msg)             \
	do {                       \
		fprintf(stderr, msg);  \
		fprintf(stderr, "\n"); \
	} while (0)

#define DEBUGF(msg, ...)                   \
	do {                                   \
		fprintf(stderr, msg, __VA_ARGS__); \
		fprintf(stderr, "\n");             \
	} while (0)

using namespace std::chrono;

/// Helper function-object to provide delta times between calls.
class time_delta_provider {
  private:
	time_point<steady_clock> last_ = steady_clock::now();

  public:
	duration<double> operator()()
	{
		auto const now = steady_clock::now();
		auto const delta = now - last_;
		last_ = now;
		return delta;
	}
};

inline ostream& operator<<(ostream& os, engine::points_status const& points)
{
	return os << get<0>(points) << ":" << get<1>(points);
}

int main(int argc, char* argv[])
{
	// game-play feedback
	auto const static on_goal = [](player player, engine::points_status points) {
		cout << "[" << points << "] Player " << player << " has scored a point." << endl;
	};
	auto const static on_game_won = [](player player, engine::points_status points) {
		cout << "[" << points << "] Player " << player << " has won this match!" << endl;
	};

	// initialize the engine; parameters are: window width, window height, goals to win
	auto game = pong::engine{dimension{1024, 768}, 8, on_goal, on_game_won};

	// create visual for ball
	auto const ball_img = canvas::colored(game.ball().size(), color::red);

	// create visual for bat (both bats have same size)
	auto const bat_img = canvas::colored(game.left_bat().size(), color::blue);

	auto main_window = window{1024, 768};
	auto delta = time_delta_provider{};

	while (main_window.handle_events() && !main_window.should_close()) {
		game.update(delta());

		if (game.over() || main_window.is_pressed(key::escape))
			break;
		if (main_window.is_pressed(key::wkey))
			game.move_left_bat(pong::bat_move::up);
		if (main_window.is_pressed(key::skey))
			game.move_left_bat(pong::bat_move::down);
		if (main_window.is_pressed(key::up))
			game.move_right_bat(pong::bat_move::up);
		if (main_window.is_pressed(key::down))
			game.move_right_bat(pong::bat_move::down);

		clear(main_window, color::black);
		pong::draw(game.left_bat(), bat_img, main_window);
		pong::draw(game.right_bat(), bat_img, main_window);
		pong::draw(game.ball(), ball_img, main_window);
		main_window.show();
	};

	return 0;
}
