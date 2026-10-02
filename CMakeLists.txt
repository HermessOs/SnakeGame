cmake_minimum_required(VERSION 3.20)
project(SnakeGame C)

set(CMAKE_C_STANDARD 11)

# FIX FOR CMAKE 4.0: Force CMake to bypass old backward-compatibility errors
set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

# FIX FOR WARNING: Set Policy CMP0135 to NEW to handle download timestamps properly
cmake_policy(SET CMP0135 NEW)

# Automatically download and configure Raylib for Windows
include(FetchContent)
FetchContent_Declare(
        raylib
        URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
)
FetchContent_MakeAvailable(raylib)

# Your main source file
add_executable(SnakeGame main.c)

# Link the libraries
target_link_libraries(SnakeGame raylib winmm)