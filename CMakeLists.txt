cmake_minimum_required(VERSION 3.13)

project(project)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/googletest" "googletest")

add_library(
        BigFloat
        BigFloatLibrary/BigFloat.cpp
        BigFloatLibrary/BigFloat.h
)

add_executable(
        program
        src/main.cpp
)

add_executable(
        test
        BigFloatLibrary/BigFloat.cpp
        tests/test.cpp
)

target_include_directories(
        test PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/BigFloatLibrary"
)

target_link_libraries(
        test PRIVATE
        gtest gtest_main
)

target_link_libraries(
        program
        BigFloat
)
