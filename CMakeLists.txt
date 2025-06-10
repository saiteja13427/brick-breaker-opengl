cmake_minimum_required(VERSION 3.10.0)
project(breakout VERSION 0.0.1 LANGUAGES C CXX)

add_executable(breakout main.cpp 
                        lib/glad.h
                        lib/glad.c 
                        lib/shader.cpp
                        lib/stb_image.cpp
                        lib/texture.cpp
                        lib/resourceman.cpp
                        lib/game.cpp
                        lib/sprite_renderer.cpp
                        lib/game_object.cpp
                        lib/game_level.cpp
                        lib/particle.cpp
                        lib/ball_object.cpp)

target_link_libraries(breakout glfw)
target_link_libraries(breakout ${CMAKE_DL_LIBS})
