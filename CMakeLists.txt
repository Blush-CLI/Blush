cmake_minimum_required(VERSION 3.20)

project(BlushCLI)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Blush
    src/main.cpp
)