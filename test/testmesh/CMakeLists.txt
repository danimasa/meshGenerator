file(GLOB SRCS *.cpp)

add_executable(testmesh ${SRCS})

target_link_libraries(testmesh meshlib ansyslib)

add_test(NAME testmesh
        COMMAND testmesh)