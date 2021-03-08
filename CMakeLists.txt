cmake_minimum_required(VERSION 3.16)
project(imgui_vge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-write-strings")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-format-security")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-int-to-void-pointer-cast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

include_directories(./)
include_directories(./src)

file(GLOB src_main
        ./src/*
        ./src/win/*
        )

add_executable(imgui_vge ${src_main})

add_subdirectory(lib_gui)
target_link_libraries(imgui_vge LINK_PUBLIC lib_gui)

