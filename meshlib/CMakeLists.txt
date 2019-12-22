file(GLOB MESHLIB_SRCS
    "*.hpp"
    "*.cpp"
)

add_library(meshlib STATIC ${MESHLIB_SRCS})
target_link_libraries(meshlib geomlib)
target_include_directories(meshlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})