cmake_minimum_required(VERSION 3.8)
project(foss)

include(cmake/auxiliary.cmake)
include(~/.vcpkg/vcpkg/scripts/buildsystems/vcpkg.cmake)

find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(imguizmo CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)

find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
include_directories(${TINYGLTF_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sources/app)
