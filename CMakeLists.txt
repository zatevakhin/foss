cmake_minimum_required(VERSION 3.8)
project(foss)

include(cmake/auxiliary.cmake)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
include_directories()
link_directories()

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(external/imgui)

add_subdirectory(sources/app)
