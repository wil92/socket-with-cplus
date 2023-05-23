cmake_minimum_required(VERSION 3.22)
project(message_test)

set(CMAKE_CXX_STANDARD 17)

# defining if compile server or client
if (DEFINED ENV{SERVER})
    message(STATUS "Compile server")
    set(CXXFLAGS "${CXXFLAGS} -DSERVER")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DSERVER")
endif()

add_executable(message_test main.cpp)
