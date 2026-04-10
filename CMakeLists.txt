cmake_minimum_required(VERSION 3.16)
project(raycaster LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(/Zc:__cplusplus)

add_subdirectory(external/glfw)
add_library(glad external/src/gl.c)
target_include_directories(glad PUBLIC external/include)

add_executable(raycaster raycaster.cpp)

target_link_libraries(raycaster PRIVATE glfw glad)