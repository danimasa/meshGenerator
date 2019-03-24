cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)

project(mesh-generator)

enable_testing()

add_subdirectory(main)
add_subdirectory(test)
add_subdirectory(geomlib)
add_subdirectory(ansyslib)
add_subdirectory(processlib)