cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(Program)
add_subdirectory(server)
add_subdirectory(shared)
add_subdirectory(client)

option(BUILD_SERVER "Build server" ON)
option(BUILD_CLIENT "Build client" OFF)

if (BUILD_SERVER AND !BUILD_CLIENT)
    target_compile_definitions(Server PRIVATE COMPILE_SERVER=1)
elseif(BUILD_CLIENT AND !BUILD_SERVER)
    target_compile_definitions(Client PRIVATE COMPILE_CLIENT=1)
endif()