
cmake_auxiliary_add_library(base_textures hpp cpp)

target_include_directories(base_textures PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(base_textures PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(base_textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)