
cmake_auxiliary_add_library(core_window hpp cpp)

target_include_directories(core_window PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_window PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)