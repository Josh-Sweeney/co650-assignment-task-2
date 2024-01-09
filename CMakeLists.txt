cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(Program)

add_subdirectory(shared)

option(BUILD_SERVER "Build server" ON)
option(BUILD_CLIENT "Build client" OFF)

if (BUILD_SERVER AND !BUILD_CLIENT)
    add_subdirectory(server)

    target_compile_definitions(Shared PRIVATE COMPILE_SERVER)
elseif(BUILD_CLIENT AND !BUILD_SERVER)
    add_subdirectory(client)

    target_compile_definitions(Shared PRIVATE COMPILE_CLIENT)
endif()