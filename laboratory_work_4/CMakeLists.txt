cmake_minimum_required(VERSION 3.16)

project(laboratory_work_4 C)

add_executable(${PROJECT_NAME} main.c)

configure_file(script.sh ${CMAKE_CURRENT_BINARY_DIR}/script.sh)