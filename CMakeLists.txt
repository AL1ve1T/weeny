cmake_minimum_required(VERSION 3.12)
project(weeny C)

set(CMAKE_C_STANDARD 11)

add_subdirectory(src)
add_subdirectory(test)

add_subdirectory(external/Unity)
target_include_directories(unity PRIVATE external/Unity)
