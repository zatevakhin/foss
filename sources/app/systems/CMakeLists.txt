
cmake_auxiliary_add_library(app_systems hpp cpp)

target_include_directories(app_systems PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(app_systems PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(app_systems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)