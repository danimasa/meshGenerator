set(SOURCE 
    main.cpp 
    KeyPoint.hpp 
    Line.hpp
    FileReader.hpp FileReader.cpp
    stringUtils.hpp
    GeomDatabase.hpp)

set(Boost_INCLUDE_DIR /usr/local/src/boost_1_67_0)
set(Boost_LIBRARY_DIR /usr/local/src/boost_1_67_0/stage/lib)
find_package(Boost 1.67 REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})