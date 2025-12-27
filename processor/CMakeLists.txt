cmake_minimum_required(VERSION 3.15)

project(spu_virtual_machine)

file(GLOB SRCS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable(spu_machine ${SRCS} ${HEADERS})

target_include_directories(
    spu_machine
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(spu_machine file_reading stack logging)
