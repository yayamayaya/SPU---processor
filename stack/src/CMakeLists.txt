cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED true)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SOURCES
    type_print.cpp
    include/stack.h
)

add_library(stack ${SOURCES})
set_target_properties(stack PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(stack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
