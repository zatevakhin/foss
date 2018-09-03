
cmake_auxiliary_add_library(core_shading hpp cpp)

target_include_directories(core_shading PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_shading PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_shading PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)