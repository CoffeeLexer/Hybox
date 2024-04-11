cmake_minimum_required(VERSION 3.22)

project(Hybox VERSION 0.1.0 LANGUAGES CXX C)

add_executable(${PROJECT_NAME} Entry.cpp)

find_package(Vulkan)

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Vulkan NOT Found")
endif ()

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan)