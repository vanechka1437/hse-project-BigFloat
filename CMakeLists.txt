cmake_minimum_required(VERSION 3.12)

project(BigFloatLibrary)

set(CMAKE_CXX_STANDARD 17)

add_executable(BigFloatLibrary BigFloat.cpp BigFloat.h
        main.cpp)
