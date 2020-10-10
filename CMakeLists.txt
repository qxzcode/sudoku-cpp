cmake_minimum_required(VERSION 3.10)

# set the project name and version
project(SudokuGen VERSION 0.1)

# specify C++ options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "-fdiagnostics-color=always -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffast-math")

# add the executable
add_executable(SudokuGen
    src/main.cpp
)
target_include_directories(SudokuGen PUBLIC "${PROJECT_BINARY_DIR}")
