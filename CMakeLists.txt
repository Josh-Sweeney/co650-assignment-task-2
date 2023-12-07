cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(Program)
add_subdirectory(server)
add_subdirectory(shared)
add_subdirectory(client)