file(GLOB GEOMLIB_SRCS
    "*.hpp"
    "*.cpp"
)

add_library(geomlib STATIC ${GEOMLIB_SRCS})
target_include_directories(geomlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})