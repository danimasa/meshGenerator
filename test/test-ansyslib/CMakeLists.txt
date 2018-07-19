file(GLOB SRCS "*.cpp")

add_executable(test-ansyslib ${SRCS})

target_link_libraries(test-ansyslib geomlib ansyslib)

add_test(NAME test-ansyslib
        COMMAND test-ansyslib)