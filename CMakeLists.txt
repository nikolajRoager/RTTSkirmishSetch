cmake_minimum_required(VERSION 4.0)
project(skirmishSketch)

#Yeah, it is pretty cool that SDL thinks it can handle main ... but no thanks
add_definitions(-DSDL_MAIN_HANDLED)

set(CMAKE_CXX_STANDARD 20)

file (COPY ${CMAKE_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})

find_package(PkgConfig REQUIRED)


pkg_check_modules(SDL2 sdl2)
pkg_check_modules(SDL2_image SDL2_image)
pkg_check_modules(SDL2_ttf SDL2_ttf)
pkg_check_modules(SDL2_mixer SDL2_mixer)
link_libraries(${SDL2_LIBRARIES})
link_libraries(${SDL2_image_LIBRARIES})
link_libraries(${SDL2_mixer_LIBRARIES})
link_libraries(${SDL2_ttf_LIBRARIES})


add_executable(skirmishSketch main.cpp
        texwrap.cpp
        texwrap.h
        soldier.cpp
        soldier.h
        projectile.cpp
        projectile.h
        base.cpp
        base.h)
