cmake_minimum_required(VERSION 4.0)
project(Project_1)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_HEADERS
        key.h
        block.h
        AES_Algorithm.h

)

set(PROJECT_SOURCES
        main.cpp
        block.cpp
)

add_executable(Project_1 ${PROJECT_SOURCES} ${PROJECT_HEADERS})