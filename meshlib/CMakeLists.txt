file(GLOB MESHLIB_SRCS
    "*.hpp"
    "*.cpp"
)

set(Boost_INCLUDE_DIR /usr/local/src/boost_1_67_0)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_67_0/stage/lib)
find_package(Boost 1.67 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_library(meshlib STATIC ${MESHLIB_SRCS})
target_link_libraries(meshlib ${Boost_LIBRARIES})
target_include_directories(meshlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})