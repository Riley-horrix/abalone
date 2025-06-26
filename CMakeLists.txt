cmake_minimum_required(VERSION 3.28)
project(abalone)
set(CMAKE_CXX_STANDARD 23)

include(CMakeSources.cmake)

include_directories(include)

add_executable(abalone_game ${SRC_FILES} abalone.cpp)