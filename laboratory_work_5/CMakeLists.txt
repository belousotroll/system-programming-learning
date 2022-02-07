project(laboratory_work_5)

add_subdirectory(5.1)
add_subdirectory(5.2)

configure_file(resource/palindrome ${CMAKE_CURRENT_BINARY_DIR}/resource/palindrome)
configure_file(resource/matrix ${CMAKE_CURRENT_BINARY_DIR}/resource/matrix)