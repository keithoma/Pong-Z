#pragma once

#include "pong_engine.hpp"

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
};

}