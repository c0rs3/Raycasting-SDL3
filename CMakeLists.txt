cmake_minimum_required(VERSION 3.16)
project(Raycaster-SDL LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Extra warnings
add_compile_options(-Wall -Wextra -pedantic)
add_compile_options(-Wno-bool-compare)
add_compile_options(-Wno-sign-compare)

# SDL3
set(SDL3_ROOT "${CMAKE_SOURCE_DIR}/dependencies/SDL3")
set(IMGUI_ROOT "${CMAKE_SOURCE_DIR}/dependencies/imgui")
set(GLAD_ROOT "${CMAKE_SOURCE_DIR}/dependencies/glad")

file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/headers/*.h")

file(GLOB IMGUI_SRC
    "${IMGUI_ROOT}/*.cpp"
    "${IMGUI_ROOT}/backends/*.cpp"
)

file(GLOB GLAD_SRC
    "${GLAD_ROOT}/src/*.c"
)

add_executable(Raycaster-SDL ${SRC_FILES} ${HEADER_FILES} ${IMGUI_SRC})

target_include_directories(Raycaster-SDL PRIVATE
    "${SDL3_ROOT}/include"
    "${CMAKE_SOURCE_DIR}/headers"
    "${IMGUI_ROOT}"
    "${IMGUI_ROOT}/backends"
    "${GLAD_ROOT}/include"
)

target_link_directories(Raycaster-SDL PRIVATE "${SDL3_ROOT}/lib")
target_link_libraries(Raycaster-SDL PRIVATE SDL3 OpenGL32)

