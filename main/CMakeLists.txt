set(SOURCE main.cpp)

add_executable(meshGenerator ${SOURCE})

target_link_libraries(meshGenerator meshlib)