cmake_minimum_required(VERSION 3.16)
project(timediff LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(timediff
    src/timediff.cpp
)

target_include_directories(timediff
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(timediff-cli
    cli/main.cpp
)

target_link_libraries(timediff-cli
    timediff
)
