cmake_minimum_required(VERSION 3.20)

project(BlushCLI)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(outside/miniaudio)

add_library(Utils STATIC
    src/utils/file.cpp
    src/utils/string.cpp
    src/utils/audio.cpp
    src/utils/input.cpp
    src/utils/saveload.cpp
)

if(WIN32)
    target_sources(Utils PRIVATE src/utils/input/windows.cpp)
else()
    target_sources(Utils PRIVATE src/utils/input/posix.cpp)
endif()


target_include_directories(Utils PRIVATE
    include
    src
    outside/miniaudio
)

target_link_libraries(Utils PRIVATE
    miniaudio
)

add_library(Core STATIC
    src/core/command.cpp
    src/core/shell.cpp
    src/core/history.cpp
)

if(WIN32)
    target_sources(Core PRIVATE src/core/job/windows.cpp)
else()
    target_sources(Core PRIVATE src/core/job/posix.cpp)
endif()

target_include_directories(Core PRIVATE
    include
    outside/miniaudio
    src
)
target_link_libraries(Core PRIVATE Utils)

add_library(Commands STATIC
    src/commands/echo.cpp
    src/commands/clear.cpp
    src/commands/play.cpp
    src/commands/exit.cpp
    src/commands/cd.cpp
    src/commands/help.cpp
    src/commands/history.cpp
    src/commands/save.cpp
)

target_include_directories(Commands PRIVATE
    include
    src
    outside/miniaudio
)
target_link_libraries(Commands PRIVATE Utils Core)

add_executable(Blush
    src/main.cpp
)

target_link_libraries(Blush PRIVATE Utils Core Commands)
target_include_directories(Blush PRIVATE
    include
    src
    outside/miniaudio
) # to what?
