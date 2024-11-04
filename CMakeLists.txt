cmake_minimum_required(VERSION 3.29)
project(DeepKnight)

set(CMAKE_CXX_STANDARD 20)

add_executable(DeepKnight main.cpp
        src/Board.cpp
        src/Board.h
        src/Types.h
)
