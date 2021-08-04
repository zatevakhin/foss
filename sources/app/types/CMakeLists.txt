
cmake_auxiliary_add_library(app_types hpp cpp)

target_include_directories(app_types PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(app_types PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(app_types PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
