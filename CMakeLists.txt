cmake_minimum_required(VERSION 3.28)
project(abalone)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DDEBUG")
set(WARNINGS
    -Wall
    -Wextra
    -Wpedantic
    -Wconversion
    -Werror
)

add_compile_options(${WARNINGS})

include(CMakeSources.cmake)

include_directories(include)
include_directories(lib/include)

add_executable(abalone_game ${SRC_FILES} abalone.cpp)