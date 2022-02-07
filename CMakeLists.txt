cmake_minimum_required(VERSION 3.0)
project(system-programming-learning)

set(CMAKE_C_STANDARD 11)

add_subdirectory(laboratory_work_4)
add_subdirectory(laboratory_work_5)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C Standard: ${CMAKE_C_STANDARD}")