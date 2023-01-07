cmake_minimum_required(VERSION 3.0.0)

# set the project name
project(serialize_test VERSION 0.1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)

include_directories(../src)

# add gtest library
find_package(GTest REQUIRED)
add_executable(serialize_test test.cpp ../src/serializer.c)
target_link_libraries(serialize_test gtest)
gtest_discover_tests(serialize_test)
