if (NOT MSVC)
	add_definitions(-Wall)
	add_definitions(-pedantic)
	add_definitions(-Werror)
endif()

add_executable(pong
	main.cpp
	object.hpp
	object.cpp
	frontend.hpp
	frontend.cpp
	pong_engine.hpp
	pong_engine.cpp
)
set_target_properties(pong PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(pong sgfx)
install(TARGETS pong DESTINATION bin)