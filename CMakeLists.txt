cmake_minimum_required(VERSION 3.15)
project(made_algo)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fno-omit-frame-pointer -mno-omit-leaf-frame-pointer")
set(CMAKE_CXX_STANDARD 14)

add_executable(task1_2 task1_2.cpp)
add_executable(task2_3 task2_3.cpp)
add_executable(task6_4 task6_4.cpp)
add_executable(task11_1 task11_1.cpp)
add_executable(test test.cpp)
