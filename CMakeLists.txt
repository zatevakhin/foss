cmake_minimum_required(VERSION 3.8)
project(foss)

include(cmake/auxiliary.cmake)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(external/anax)
add_subdirectory(external/imgui)

# add_subdirectory(sources/core)
add_subdirectory(sources/app)
