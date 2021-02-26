cmake_minimum_required(VERSION 3.16)
project(lb_manager)

#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wundefined-internal")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -static-libstdc++ -static-libgcc")

include_directories(./)
include_directories(softael_lib/)
include_directories(lib/imgui/)
include_directories(lib/opengl/)
include_directories(lib/implot/)
include_directories(lib/hidapi/)

file(GLOB src_main      "src/*.*" "src/win/*.*")
file(GLOB src_imgui     "lib/imgui/*.*" "lib/imgui/win/*.*")
#file(GLOB src_libhid    "lib/libhid/win32/*.cc")
#file(GLOB src_hidapi    "lib/hidapi/windows/*.*")
file(GLOB src_opengl    "lib/opengl/*.*")
#file(GLOB src_tinymat   "lib/tinymat/*.*")
file(GLOB src_implot    "lib/implot/*.*")

add_executable(lb_manager ${src_main} ${src_imgui} ${src_opengl} ${src_tinymat} ${src_implot} ${src_hidapi})

target_link_libraries(lb_manager LINK_PUBLIC opengl32)
target_link_libraries(lb_manager LINK_PUBLIC ../lib/opengl/static/glfw3_x64)
target_link_libraries(lb_manager LINK_PUBLIC ../lib/hidapi/windows/hidapi_x64/hidapi)

