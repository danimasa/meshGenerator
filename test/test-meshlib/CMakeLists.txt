file(GLOB SRCS "*.cpp")

add_executable(test-meshlib ${SRCS})

target_link_libraries(test-meshlib meshlib)

add_test(NAME test-meshlib
        COMMAND test-meshlib)