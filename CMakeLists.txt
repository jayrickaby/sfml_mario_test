cmake_minimum_required(VERSION 4.1)
project(sfml_mario_test)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)
add_subdirectory(dependencies)


install(TARGETS sfml_mario_test RUNTIME DESTINATION bin)

# Copy Assets Folder
#add_custom_command(TARGET sfml_mario_test POST_BUILD
#        COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/bin/assets
#)
