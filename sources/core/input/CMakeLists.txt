
cmake_auxiliary_add_library(core_input hpp cpp)

target_include_directories(core_input PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_input PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
