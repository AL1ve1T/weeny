cmake_minimum_required(VERSION 3.21)
project(weeny C)

set(CMAKE_C_STANDARD 11)

include_directories(libs/Unity/src)

include_directories(src/test)

add_executable(weeny src/main.c src/test/main_test.c src/defines.h src/run_tests.c src/server.c)
