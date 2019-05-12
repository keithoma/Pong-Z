// This file is part of the "pong" project, http://github.com/keithoma/pong>
//   (c) 2019-2019 Christian Parpart <christian@parpart.family>
//   (c) 2019-2019 Kei Thoma <thomakmj@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#include "frontend.hpp"

#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <array>
#include <chrono>
#include <functional>
#include <iostream>

using namespace std;
using namespace std::chrono;
using namespace std::placeholders;
using namespace sgfx;

namespace {

inline std::ostream& operator<<(std::ostream& os, pong::player p)
{
	static constexpr array<char const*, 2> names = {"left", "right"};
	return os << names.at(static_cast<size_t>(p));
}

inline ostream& operator<<(ostream& os, pong::engine::points_status const& points)
{
	return os << get<0>(points) << ":" << get<1>(points);
}

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

}  // namespace

namespace pong {

frontend::frontend()
	: game_{dimension{1024, 768}, 8, bind(&frontend::on_goal, this, _1, _2),
			bind(&frontend::on_game_won, this, _1, _2)},
	  main_window_{1024, 768},
	  ball_image_{canvas::colored(game_.ball().size(), color::red)},
	  bat_image_{canvas::colored(game_.left_bat().size(), color::blue)},
	  scores_left_{game_.max_points()},
	  scores_right_{game_.max_points()}
{
}

void frontend::run()
{
	auto delta = time_delta_provider{};

	while (handle_events()) {
		game_.update(delta());
		render_scene();
	};
}

bool frontend::handle_events()
{
	if (!main_window_.handle_events() || main_window_.should_close() || main_window_.is_pressed(key::escape))
		return false;

	if (main_window_.is_pressed(key::backspace)) {
		game_.reset();
		scores_left_ = 0;
		scores_right_ = 0;
	}

	if (game_.over()) {
		game_.freeze();	
		return true;
	}

	if (main_window_.is_pressed(key::wkey))
		game_.move_left_bat(pong::bat_move::up);

	if (main_window_.is_pressed(key::skey))
		game_.move_left_bat(pong::bat_move::down);

	if (main_window_.is_pressed(key::up))
		game_.move_right_bat(pong::bat_move::up);

	if (main_window_.is_pressed(key::down))
		game_.move_right_bat(pong::bat_move::down);

	return true;
}

void frontend::render_scene()
{
	clear(main_window_, color::black);

	draw(game_.left_bat(), bat_image_, main_window_);
	draw(game_.right_bat(), bat_image_, main_window_);
	draw(game_.ball(), ball_image_, main_window_);

	// sbp = score board position
	auto constexpr sbp_padding_horizontal = 10;
	auto constexpr sbp_top = 10;
	auto const sbp_left = point{main_window_.width() / 2 - scores_left_.width() - sbp_padding_horizontal, sbp_top};
	auto const sbp_right = point{main_window_.width() / 2 + sbp_padding_horizontal, sbp_top};

	draw(main_window_, scores_left_, sbp_left);
	draw(main_window_, scores_right_, sbp_right);

	main_window_.show();
}

void frontend::on_goal(player _player, engine::points_status _points)
{
	if (_player == player::left)
		++scores_left_;
	else
		++scores_right_;
}

void frontend::on_game_won(player player, engine::points_status points)
{
	// TODO: boah
}

}  // namespace pong
