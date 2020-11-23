cmake_minimum_required(VERSION 3.17)
project(UDPClient C)

set(CMAKE_C_STANDARD 11)
include_directories(~/CLionProjects/Project1113/graphics)

find_library(LibGraphics libgraphics.a ~/CLionProjects/Project1113/graphics/cmake-build-debug/)
add_executable(UDPClient main.c )
target_link_libraries(UDPClient  ${LibGraphics} m config)