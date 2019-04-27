#include <sgfx/color.hpp>
#include <sgfx/window.hpp>


int main(int argc, char* argv[])
{
	using namespace sgfx;
	
	window main_window{256,256};
	int x=0,y=0;
	
	while(main_window.handle_events() && !main_window.should_close())
	{
		if(main_window.is_pressed(key::down))
			++y;
			
		if(main_window.is_pressed(key::right))
			++x;
			
		x%=main_window.width();
		y%=main_window.height();
			
		main_window.pixels()[y*main_window.width()+x]=color::white;
		main_window.show();
	}
	return 0;
}
