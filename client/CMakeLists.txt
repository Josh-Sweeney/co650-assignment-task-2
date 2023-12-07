cmake_minimum_required(VERSION 3.15)

project(Client)

set(CMAKE_CXX_STANDARD 17)

add_executable(Client
    src/main.cpp
)

target_link_libraries(Client PRIVATE ws2_32)