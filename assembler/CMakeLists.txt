cmake_minimum_required(VERSION 3.15)

project(assembler)

file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h)

add_executable(assembler ${SOURCES} ${HEADERS})

target_link_libraries(assembler file_reading)

target_include_directories(
    assembler
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)