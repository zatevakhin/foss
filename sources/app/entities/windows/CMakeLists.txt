
cmake_auxiliary_add_library(app_entities_windows hpp cpp)

target_include_directories(app_entities_windows PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(app_entities_windows PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(app_entities_windows PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
