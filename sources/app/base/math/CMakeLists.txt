
cmake_auxiliary_add_library(base_math hpp cpp)

target_include_directories(base_math PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(base_math PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(base_math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
