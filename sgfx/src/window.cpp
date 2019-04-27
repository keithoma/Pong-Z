#include <sgfx/window.hpp>

#include <stdexcept>

namespace
{
	const char* vertex_shader_source=
	R"(#version 120
		
		attribute vec2 vertices;
		varying vec2 tex_coord;

		void main()
		{
			vec2 tmp=(vertices+vec2(1,1))/2;
			tex_coord=vec2(tmp[0],-(tmp[1]-1));
			gl_Position=vec4(vertices,0, 1);
		}
	)";
	
	const char* fragment_shader_source=
	R"(#version 120
		uniform sampler2D tex;
		varying vec2 tex_coord;

		void main()
		{
			gl_FragColor = texture2D(tex, tex_coord);
		}
	)";
	
	const auto& init()
	{
		static const struct glfw_init_raii
		{
			glfw_init_raii()
			{
				if(!glfwInit())
					throw std::runtime_error("Failed to init gflw");
			}
			
			~glfw_init_raii()
			{
				glfwTerminate();
			}
			
		} init_object;
		
		return init_object;
	}
	
	void compose_shaders(GLuint& vao_id, GLuint& vbo_id)
	{
		static GLfloat vertices[]=
		{
			-1,  1,
			-1, -1,
			 1, -1,
			
			 1, -1,
			 1,  1,
			-1,  1,
		};

		const auto check_shader_compilation=[](auto id)
		{
			GLint success=0;
			glGetShaderiv(id,GL_COMPILE_STATUS,&success);
			if(success==GL_FALSE)
			{
				GLint error_len=0;
				glGetShaderiv(id,GL_INFO_LOG_LENGTH,&error_len);
				std::vector<char> error(error_len);
				glGetShaderInfoLog(id,error_len,&error_len,error.data());
				throw std::runtime_error(std::string{"Failed to compile shader, error was:\n"}+error.data());
			}
		};
		
		GLuint vertex_shader_id=glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader_id, 1, &vertex_shader_source, nullptr);
		glCompileShader(vertex_shader_id);

		GLuint fragment_shader_id=glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader_id, 1, &fragment_shader_source, nullptr);
		glCompileShader(fragment_shader_id);
		
		check_shader_compilation(vertex_shader_id);
		check_shader_compilation(fragment_shader_id);

		GLuint shader_program_id=glCreateProgram();
		glAttachShader(shader_program_id,vertex_shader_id);
		glAttachShader(shader_program_id,fragment_shader_id);
		
		glDeleteShader(vertex_shader_id);
		glDeleteShader(fragment_shader_id);

		glLinkProgram(shader_program_id);
		glUseProgram(shader_program_id);
		
		glGenVertexArrays(1,&vao_id);
		glBindVertexArray(vao_id);
	
		glGenBuffers(1,&vbo_id);
		glBindBuffer(GL_ARRAY_BUFFER,vbo_id);
		glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
		
		glEnableVertexAttribArray(glGetAttribLocation(shader_program_id,"vertices"));
		glVertexAttribPointer(glGetAttribLocation(shader_program_id,"vertices"),2,GL_FLOAT,GL_FALSE,2*sizeof(GLfloat),nullptr);
		
		glUniform1i(glGetUniformLocation(shader_program_id, "tex"),0);
	}
}

using namespace sgfx;

window::window(std::uint16_t w, std::uint16_t h, const char* title):
	width_{w},
	height_{h},
	pixels_(w*h)
{
	init();
	
	wnd_=glfwCreateWindow(width_, height_, title, nullptr, nullptr);
	if(!wnd_)
		throw std::runtime_error("Failed to create window");
		
	glfwMakeContextCurrent(wnd_);

	glViewport(0,0,width_,height_);
	glfwSetFramebufferSizeCallback(wnd_,[](GLFWwindow* wnd, int width, int height)
	{
		glfwMakeContextCurrent(wnd);
		glViewport(0,0,width,height);
	});
	
	if(glewInit()!=GLEW_OK)
		throw std::runtime_error("Failed to init glew");
	compose_shaders(vao_id_,vbo_id_);
	
	glGenTextures(1,&texture_id_);
	glBindTexture(GL_TEXTURE_2D,texture_id_);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width_,height_,0,GL_RGB,GL_UNSIGNED_BYTE,pixels_.data());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

window::~window()
{
	glfwDestroyWindow(wnd_);
	glDeleteVertexArrays(1,&vao_id_);
	glDeleteBuffers(1,&vbo_id_);
	glDeleteTextures(1,&texture_id_);
}

bool window::handle_events()
{
	glfwPollEvents();
	return true;
}

bool window::should_close() const
{
	return glfwWindowShouldClose(wnd_);
}

bool window::is_pressed(key_id key) const
{
	return glfwGetKey(wnd_,key.id_)==GLFW_PRESS;
}

void window::show() const
{
	glfwMakeContextCurrent(wnd_);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_id_);
	glTexSubImage2D(GL_TEXTURE_2D,0,0,0,width_,height_,GL_RGB,GL_UNSIGNED_BYTE,pixels_.data());
	
	glBindVertexArray(vao_id_);
	
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glfwSwapBuffers(wnd_);
}
