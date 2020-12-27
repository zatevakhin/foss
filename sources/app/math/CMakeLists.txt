
cmake_auxiliary_add_library(app_math hpp cpp)

target_include_directories(app_math PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(app_math PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(app_math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
