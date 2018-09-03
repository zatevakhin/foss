
cmake_auxiliary_add_library(core_resources hpp cpp)

target_include_directories(core_resources PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_resources PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_resources PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)