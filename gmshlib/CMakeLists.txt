file(GLOB GMSHLIB_SRCS
    "*.hpp"
    "*.cpp"
)

add_library(gmshlib STATIC ${GMSHLIB_SRCS})
target_link_libraries(gmshlib geomlib meshlib)
target_include_directories(gmshlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})