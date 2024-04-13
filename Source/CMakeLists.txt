add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/Window)

add_executable(HyboxApp ${CMAKE_CURRENT_LIST_DIR}/Entry/App.cpp)

target_link_libraries(HyboxApp PUBLIC Window)
target_include_directories(HyboxApp PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Window)
