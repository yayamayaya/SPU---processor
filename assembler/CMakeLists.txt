cmake_minimum_required(VERSION 3.15)

project(assembler)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/include/*.h)

add_executable(assembler ${SRCS} ${HEADERS})

target_include_directories(
    assembler
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/include
)

target_link_libraries(assembler file_reading stack logging)
