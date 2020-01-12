
cmake_auxiliary_add_library(base_shading hpp cpp)

target_include_directories(base_shading PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(base_shading PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(base_shading PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)