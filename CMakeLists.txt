cmake_minimum_required(VERSION 3.14)
project(HW_1)

set(CMAKE_CXX_STANDARD 14)
find_package (Threads)
add_executable(HW_1 main.cpp)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O2")
target_link_libraries(HW_1 ${CMAKE_THREAD_LIBS_INIT})