file(GLOB PROCESSLIB_SRCS
    "*.hpp"
    "*.cpp"
)

find_package(CGAL)

add_library(processlib STATIC ${PROCESSLIB_SRCS})
target_link_libraries(processlib geomlib CGAL::CGAL)
target_include_directories(processlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})