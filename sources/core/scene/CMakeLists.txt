
cmake_auxiliary_add_library(core_scene hpp cpp)

target_include_directories(core_scene PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_scene PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_scene PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)