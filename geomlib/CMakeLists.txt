file(GLOB GEOMLIB_SRCS
    "*.hpp"
    "*.cpp"
)

find_package(Boost 1.65 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

if (WIN32)
    find_package(Armadillo CONFIG REQUIRED)
else()
    find_package(Armadillo REQUIRED)
endif (WIN32)

include_directories(${ARMADILLO_INCLUDE_DIRS})

find_package(CGAL)

add_library(geomlib STATIC ${GEOMLIB_SRCS})
target_link_libraries(geomlib ${Boost_LIBRARIES} ${ARMADILLO_LIBRARIES} CGAL::CGAL)
target_include_directories(geomlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})