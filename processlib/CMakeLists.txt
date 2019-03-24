file(GLOB PROCESSLIB_SRCS
    "*.hpp"
    "*.cpp"
)

add_library(processlib STATIC ${PROCESSLIB_SRCS})
target_link_libraries(processlib geomlib)
target_include_directories(processlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})