cmake_minimum_required(VERSION 3.15)

set(SOURCES
    stack_type_funcs.cpp
    stack_ext_func.hpp
    stack_itern_func.hpp
    stack_log.h
    stack.h
)

add_library(stack ${SOURCES})
target_include_directories(stack PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
