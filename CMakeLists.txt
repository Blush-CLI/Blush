cmake_minimum_required(VERSION 3.20)

project(BlushCLI)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(Utils STATIC
    src/utils/file.cpp
    src/utils/string.cpp
)
target_include_directories(Utils PRIVATE
    include
    src
)

add_library(Core STATIC
    src/core/command.cpp
    src/core/shell.cpp
)
target_include_directories(Core PRIVATE
    include
    src
)
target_link_libraries(Core PRIVATE Utils)

add_executable(Blush
    src/main.cpp
)

target_link_libraries(Blush PRIVATE Utils Core)
target_include_directories(Blush PRIVATE
    include
    src
)
