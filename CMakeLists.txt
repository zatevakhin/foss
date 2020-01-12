cmake_minimum_required(VERSION 3.8)
project(foss)

include(cmake/auxiliary.cmake)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/imgui)

add_subdirectory(sources/app)
