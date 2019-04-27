#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/key.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <algorithm>

int main(int argc, char* argv[])
{
	using namespace sgfx;
	
	window main_window{1024,768};
	
	canvas img{{64,64}};
	clear(img,color::white);
	line(img,{0,0},{64,64},color::green);
	line(img,{64,0},{0,64},color::green);
	
	rectangle center_rect{{img.width()/2-16,img.height()/2-16},{32,32}};
	fill(img,center_rect,color::blue);
	
	point pos{main_window.width()/2-img.width()/2,main_window.height()/2-img.height()/2};
	while(main_window.handle_events() && !main_window.should_close())
	{
		if(main_window.is_pressed(key_id{'w'}))
			pos.y-=1;
		if(main_window.is_pressed(key_id{'s'}))
			pos.y+=1;
			
		if(main_window.is_pressed(key_id{'a'}))
			pos.x-=1;
		if(main_window.is_pressed(key_id{'d'}))
			pos.x+=1;
			
		if(main_window.is_pressed(key::escape))
			break;
		
		clear(main_window,color::black);
		draw(main_window,img,pos);
		
		main_window.show();
	};

	return 0;
}
