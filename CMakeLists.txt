cmake_minimum_required(VERSION 3.20)
project(GtaPowerEssential CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dinput8 SHARED
    src/main.cpp
    src/memory.cpp
    src/renderer.cpp
    src/hook.hpp
)

set_target_properties(dinput8 PROPERTIES
    OUTPUT_NAME "dinput8"
    SUFFIX ".dll"
)
