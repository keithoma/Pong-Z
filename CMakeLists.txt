cmake_minimum_required(VERSION 3.0)
project(PongGame CXX)

cmake_policy(SET CMP0028 NEW)

option(SGFX_EXAMPLES "Build SGFX examples" ON)

add_subdirectory(sgfx)
add_subdirectory(pong)

if(SGFX_EXAMPLES)
	add_subdirectory(examples/basics)
	add_subdirectory(examples/primitives)
endif()
