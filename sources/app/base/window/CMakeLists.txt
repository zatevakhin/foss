
cmake_auxiliary_add_library(base_window hpp cpp)

target_include_directories(base_window PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(base_window PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(base_window PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)