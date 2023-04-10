cmake_minimum_required(VERSION 3.20)
set(CMAKE_C_COMPILER "/usr/bin/clang")
project(sdl2048 LANGUAGES C)

set(CMAKE_C_STANDARD 11)

#add_compile_options("-O3")
#add_link_options("-s")

file(GLOB sources CONFIGURE_DEPENDS "src/*.c" "src/*.h")
add_executable(sdl2048 ${sources})

include_directories(libs/sdl/include)
file(GLOB sdl_binaries CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/libs/sdl/bin/*")
target_link_libraries(${PROJECT_NAME} ${sdl_binaries})

include_directories(libs/sdl_ttf/include) # changed '#include "*.h"' to '#include <sdl/*.h>' in 'SDL_ttf.h'
file(GLOB sdl_ttf_binaries CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/libs/sdl_ttf/bin/*")
target_link_libraries(${PROJECT_NAME} ${sdl_ttf_binaries})

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
