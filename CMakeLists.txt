cmake_minimum_required(VERSION 3.15)
project(dinput8_framework CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(dinput8 SHARED
    src/main.cpp
)

include(FetchContent)
FetchContent_Declare(
  pugixml
  GIT_REPOSITORY https://github.com/zeux/pugixml.git
  GIT_TAG v1.14
)
FetchContent_MakeAvailable(pugixml)

target_link_libraries(dinput8 PRIVATE pugixml::pugixml)
