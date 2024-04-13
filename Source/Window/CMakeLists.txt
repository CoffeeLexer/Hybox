project(Window VERSION 0.1.0 DESCRIPTION "Windowing system" LANGUAGES CXX)

add_library(Window SHARED GLFW.cpp)

find_package(glfw3 REQUIRED 3.3)

target_link_libraries(Window PUBLIC glfw)
