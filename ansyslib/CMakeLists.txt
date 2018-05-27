file(GLOB ANSYSLIB_SRCS
    "*.hpp"
    "*.cpp"
)
# set(ANSYSLIB_SRCS AnsysFileReaderFactory.hpp AnsysFileReaderFactory.cpp)

add_library(ansyslib STATIC ${ANSYSLIB_SRCS})
target_link_libraries(ansyslib meshlib)
target_include_directories(ansyslib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})