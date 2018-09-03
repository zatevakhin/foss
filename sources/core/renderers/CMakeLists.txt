
cmake_auxiliary_add_library(core_renderers hpp cpp)

target_include_directories(core_renderers PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_renderers PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_renderers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)