cmake_minimum_required(VERSION 3.15)
project(RaycasterEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
  GIT_TAG main
)

FetchContent_Declare(
  SDL3_image
  GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
  GIT_TAG main
)

FetchContent_MakeAvailable(SDL3 SDL3_image)

file(GLOB ENGINE_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
add_library(raycaster_engine STATIC ${ENGINE_SOURCES})

target_include_directories(raycaster_engine PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/headers
)

target_link_libraries(raycaster_engine PUBLIC
  SDL3::SDL3
  SDL3_image::SDL3_image
)

target_compile_definitions(raycaster_engine PRIVATE
  $<$<CONFIG:Debug>:ENGINE_DEBUG>
  $<$<CONFIG:Release>:NDEBUG>
)

target_compile_options(raycaster_engine PRIVATE
  $<$<CONFIG:Debug>:-g;-O0>
  $<$<CONFIG:Release>:-O3>
)