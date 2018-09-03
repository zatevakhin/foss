function(cmake_auxiliary_add_library TARGET HEADERS_EXT SOURCES_EXT)
    file(GLOB TARGET_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/*.${HEADERS_EXT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.${SOURCES_EXT}"
        )
    add_library(${TARGET} ${TARGET_SRC})
endfunction()

function(cmake_auxiliary_add_execultable TARGET HEADERS_EXT SOURCES_EXT)
    file(GLOB TARGET_SRC
        "${CMAKE_CURRENT_SOURCE_DIR}/*.${HEADERS_EXT}"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.${SOURCES_EXT}"
        )
    add_executable(${TARGET} ${TARGET_SRC})
endfunction()