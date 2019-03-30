file(GLOB SRCS "*.cpp")

add_executable(test-processlib ${SRCS})

target_link_libraries(test-processlib geomlib processlib)

add_test(NAME test-processlib
        COMMAND test-processlib)