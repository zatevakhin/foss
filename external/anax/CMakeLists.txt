
file(GLOB TARGET_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/util/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/*.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/*.cpp"
)
add_library(anax ${TARGET_SRC})

target_include_directories(anax PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../)
target_include_directories(anax PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/detail/)