#include <sgfx/canvas.hpp>
#include <sgfx/color.hpp>
#include <sgfx/primitives.hpp>
#include <sgfx/window.hpp>

#include <algorithm>

int main(int argc, char* argv[])
{
	using namespace sgfx;
	
	window main_window{1024,768};
	
	canvas img0{{50,50}};
	std::fill_n(img0.pixels(), img0.width()*img0.height()/2, color::gray);
	std::fill_n(img0.pixels()+img0.width()*img0.height()/2, img0.width()*img0.height()/2, color::cyan);
	
	auto img1=img0;
	std::fill_n(img1.pixels()+img1.width()*img1.height()/2, img1.width()*img1.height()/2, color::magenta);
	
	point pos0{0,0}, pos1{main_window.width()-img1.width(),0};
	int dx0=4, dx1=-4;
	while(pos0.y<main_window.height())
	{
		clear(main_window,color::black);
		draw(main_window,img0,pos0);
		draw(main_window,img1,pos1);
		
		main_window.show();
		
		pos0.y=pos1.y=pos0.y+1;
		
		pos0.x+=dx0;
		pos1.x+=dx1;
		
		if(pos0.x<=0)
			dx0=4;
		if(pos0.x>=main_window.width())
			dx0=-4;
			
		dx1=-dx0;
	};

	return 0;
}
