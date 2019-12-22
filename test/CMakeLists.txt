set(SRCS catch.hpp fakeit.hpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(test-ansyslib)
add_subdirectory(test-geomlib)
add_subdirectory(test-processlib)
add_subdirectory(test-meshlib)