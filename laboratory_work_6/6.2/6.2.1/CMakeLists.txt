project(6_2_1 C)
add_executable(${PROJECT_NAME} main.c)

configure_file(resource/matrix.txt ${CMAKE_CURRENT_BINARY_DIR}/resource/matrix.txt)
