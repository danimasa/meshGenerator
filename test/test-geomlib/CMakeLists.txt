file(GLOB SRCS "*.cpp")

add_executable(test-geomlib ${SRCS})

target_link_libraries(test-geomlib geomlib)

add_test(NAME test-geomlib
        COMMAND test-geomlib)