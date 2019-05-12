if (NOT MSVC)
	add_definitions(-Wall)
	add_definitions(-pedantic)
	add_definitions(-Werror)
endif()

add_executable(pong_game
	main.cpp
	object.hpp
	object.cpp
	frontend.hpp
	frontend.cpp
	pong_engine.hpp
	pong_engine.cpp
	score_board.hpp
	score_board.cpp
)
set_target_properties(pong_game PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
target_link_libraries(pong_game sgfx)
install(TARGETS pong_game DESTINATION bin)