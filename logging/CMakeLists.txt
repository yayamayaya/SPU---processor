cmake_minimum_required(VERSION 3.5)

add_library(logging log.cpp)
target_include_directories(logging PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
