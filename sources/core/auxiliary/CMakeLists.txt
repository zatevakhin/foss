
cmake_auxiliary_add_library(core_auxiliary hpp cpp)

target_include_directories(core_auxiliary PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_auxiliary PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_auxiliary PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)