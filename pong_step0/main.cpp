#include "ball.hpp"

#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

int main(int argc, char* argv[])
{
	using namespace sgfx;
	using namespace pong;
	
	window main_window{1024,768};
	rectangle bounds{{10,10},{static_cast<std::uint16_t>(main_window.width()-20), static_cast<std::uint16_t>(main_window.height()-20)}};
	
	ball single_ball;
	single_ball.bind_to(bounds);
	single_ball.limit_velocity({6,6});
	single_ball.move_to({main_window.width()/2, main_window.height()/2});
	single_ball.accelerate({1,2});
	
	while(main_window.handle_events() && !main_window.should_close())
	{
		switch(single_ball.update_step())
		{
			case ball::status::stuck_left:
			case ball::status::stuck_right:
				single_ball.reflect_x();
				break;
			case ball::status::stuck_top:
			case ball::status::stuck_bottom:
				single_ball.reflect_y();
				break;
			case ball::status::free:
				break;
			default:
				single_ball.reflect_x();
				single_ball.reflect_y();
		}
		
		if(main_window.is_pressed(key::escape))
			break;
		
		clear(main_window,color::black);
		single_ball.draw(main_window);
		
		main_window.show();
	};

	return 0;
}
