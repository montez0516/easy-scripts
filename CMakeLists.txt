cmake_minimum_required(VERSION 4.0.0)


project(easyscripts)

add_executable(runner
main.cpp 
core/process/process.cpp
core/runner/engine/runnerEngine.cpp 
core/runner/runtime/runtimes/pythonRuntime.cpp
core/runner/runtime/runtimeManager.cpp
)

target_include_directories(runner INTERFACE core/process core/runner core/runner/engine core/runner/runtime core/runner/runtime/runtimes)