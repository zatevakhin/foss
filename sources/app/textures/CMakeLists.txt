
cmake_auxiliary_add_library(app_textures hpp cpp)

target_include_directories(app_textures PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(app_textures PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(app_textures PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
