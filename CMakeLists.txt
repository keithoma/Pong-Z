cmake_minimum_required(VERSION 3.0)
project(CxxAsNewLang)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sgfx)
add_subdirectory(examples/basics)
add_subdirectory(examples/primitives)

# Only include the one below when working on them and have them compiling
#add_subdirectory(pong_step0)
#add_subdirectory(pong_step1)

# Small test program for demo purposes and quick'n'dirty tests.
add_executable(myprog myprog.cpp)