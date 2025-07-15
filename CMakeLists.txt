cmake_minimum_required(VERSION 3.16)
project(Raycaster-SDL)

set(CMAKE_CXX_STANDARD 20)

set(SDL3_ROOT "${CMAKE_SOURCE_DIR}/SDL3")

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

include_directories("${SDL3_ROOT}/include")

link_directories("${SDL3_ROOT}/lib")

add_executable(Raycaster-SDL "main.cpp")

target_link_libraries(Raycaster-SDL SDL3)