file(GLOB ANSYSLIB_SRCS
    "*.hpp"
    "*.cpp"
)

find_package(Boost 1.67 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_library(ansyslib STATIC ${ANSYSLIB_SRCS})
target_link_libraries(ansyslib geomlib ${Boost_LIBRARIES})
target_include_directories(ansyslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})