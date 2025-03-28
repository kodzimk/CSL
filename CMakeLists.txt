cmake_minimum_required(VERSION 3.5)

project (CSL)

file(GLOB SOURCES
    src/*.h
    src/*.cpp
    main.csl
)
 set(CMAKE_CXX_STANDARD 20)


add_executable(CSL ${SOURCES})