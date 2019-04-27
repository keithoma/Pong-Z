#include "object.hpp"

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
	
	canvas ball_img{{20,20}};
	clear(ball_img,color::red);
	
	canvas bat_img{{20,100}};
	clear(bat_img,color::blue);
	
	object ball{ball_img};
	ball.bind_to(bounds);
	ball.limit_velocity({6,6});
	ball.move_to({main_window.width()/2, main_window.height()/2});
	ball.accelerate({1,2});
	
	object bat{bat_img};
	bat.move_to({0,400});
	bat.limit_velocity({0,6});
	bat.bind_to({{0,0},{20,768}});
	
	while(main_window.handle_events() && !main_window.should_close())
	{
		switch(ball.update_step())
		{
			case object::status::stuck_left:
			case object::status::stuck_right:
				ball.reflect_x();
				break;
			case object::status::stuck_top:
			case object::status::stuck_bottom:
				ball.reflect_y();
				break;
			case object::status::free:
				break;
			default:
				ball.reflect_x();
				ball.reflect_y();
		}
		
		bat.update_step();
		
		if(main_window.is_pressed(key::escape))
			break;
		
		if(main_window.is_pressed(key::up))
			bat.accelerate({0,-1});
		if(main_window.is_pressed(key::down))
			bat.accelerate({0,1});
		
		
		clear(main_window,color::black);
		bat.draw(main_window);
		ball.draw(main_window);
		
		main_window.show();
	};

	return 0;
}
