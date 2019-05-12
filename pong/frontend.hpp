// This file is part of the "pong" project, http://github.com/keithoma/pong>
//   (c) 2019-2019 Christian Parpart <christian@parpart.family>
//   (c) 2019-2019 Kei Thoma <thomakmj@gmail.com>
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT

#pragma once

#include "pong_engine.hpp"
#include "score_board.hpp"

#include <sgfx/canvas.hpp>
#include <sgfx/window.hpp>

namespace pong {

class frontend {
  public:
	frontend();

	void run();

  private:
	bool handle_events();
	void render_scene();
	void on_goal(player player, engine::points_status points);
	void on_game_won(player player, engine::points_status points);

  private:
	engine game_;
	sgfx::window main_window_;
	sgfx::canvas ball_image_;
	sgfx::canvas bat_image_;
	score_board scores_left_;
	score_board scores_right_;
};

}
