#ifndef SGFX_WINDOW_H
#define SGFX_WINDOW_H

#include <sgfx/color.hpp>
#include <sgfx/key.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>

#include <cctype>
#include <cstdint>

namespace sgfx
{
	class window
	{
		public:
		window(std::uint16_t w, std::uint16_t h):
			window(w,h,"Default")
		{}
		
		window(std::uint16_t w, std::uint16_t h, const char* title);
		
		~window();
		
		window(const window&)=delete;
		window& operator=(const window&)=delete;
		
		bool handle_events();
		bool should_close() const;
		bool is_pressed(key_id key) const;
		
		void show() const;
		
		std::uint16_t width() const { return width_; }
		std::uint16_t height() const { return height_; }
		
		auto pixels() { return pixels_.data(); }
		
		private:
		GLFWwindow* wnd_;
		GLuint texture_id_;
		GLuint vao_id_;
		GLuint vbo_id_;

		const std::uint16_t width_, height_;
		std::vector<color::rgb_color> pixels_;
	};
}

#endif
