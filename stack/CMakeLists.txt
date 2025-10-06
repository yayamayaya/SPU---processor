cmake_minimum_required(VERSION 3.15)

add_subdirectory(src)

target_include_directories(
    stack
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)