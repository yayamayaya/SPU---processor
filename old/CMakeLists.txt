cmake_minimum_required(VERSION 3.15)

project(processor)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

add_executable(processor ${SOURCES} ${HEADERS})

target_include_directories(
    processor
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_link_libraries(processor file_reading stack)