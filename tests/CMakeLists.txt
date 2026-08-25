cmake_minimum_required(VERSION 4.0.0)


project(test)

add_executable(test test.cpp)

target_include_directories(test INTERFACE ../src/core/eventBus)