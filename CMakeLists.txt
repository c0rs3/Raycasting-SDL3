cmake_minimum_required(VERSION 3.15)
project(RaycasterSDLProject LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(RaycasterSDL)

add_executable(raycaster_main main.cpp)

target_link_libraries(raycaster_main PRIVATE raycaster_engine)