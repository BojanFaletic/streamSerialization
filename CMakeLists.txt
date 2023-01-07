# buld project with CMake
cmake_minimum_required(VERSION 3.0.0)
project(serialize VERSION 0.1.0)

# set the C standard
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

# include directories
include_directories(src)

# enable gtest library
enable_testing()
add_subdirectory(testing)

# main executable
add_executable(serialize example.c src/serializer.c)