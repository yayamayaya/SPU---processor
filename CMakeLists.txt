cmake_minimum_required(VERSION 3.15)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/assembler/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/common/file_reading/src
    ${CMAKE_CURRENT_SOURCE_DIR}/common/logging
    ${CMAKE_CURRENT_SOURCE_DIR}/stack/src
)

add_definitions(-Wall -g)
add_definitions(-DDEBUG -DSECURE)

add_subdirectory(assembler)
add_subdirectory(processor)
add_subdirectory(common/file_reading ${CMAKE_CURRENT_BINARY_DIR}/file_reading)
add_subdirectory(stack ${CMAKE_CURRENT_BINARY_DIR}/stack)