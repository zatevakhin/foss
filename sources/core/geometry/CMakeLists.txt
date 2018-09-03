
cmake_auxiliary_add_library(core_geometry hpp cpp)

target_include_directories(core_geometry PUBLIC ${PROJECT_SOURCE_DIR}/external/)
target_include_directories(core_geometry PUBLIC ${PROJECT_SOURCE_DIR}/sources/)
target_include_directories(core_geometry PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)