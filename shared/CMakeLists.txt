cmake_minimum_required(VERSION 3.15)

project(Shared)

set(CMAKE_CXX_STANDARD 17)

add_library(Shared
    src/server.h
    src/server.cpp
    src/client.h
    src/client.cpp
    src/comms.h
    src/comms.cpp
)

target_link_libraries(Shared PRIVATE ws2_32)