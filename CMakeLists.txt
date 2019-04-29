cmake_minimum_required(VERSION 3.0)
project(CxxAsNewLang)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(sgfx)
add_subdirectory(examples/basics)

# Small test program for demo purposes and quick'n'dirty tests.
add_executable(myprog myprog.cpp)