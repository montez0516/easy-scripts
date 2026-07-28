cmake_minimum_required(VERSION 4.0.0)


project(runner)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

set(SPDLOG_BUILD_STATIC ON CACHE INTERNAL "")
FetchContent_MakeAvailable(spdlog)

add_executable(runner
src/runner.cpp 
src/core/process/process.cpp
src/core/ipc/namedPipe.cpp
src/core/ipc/unnamedPipe.cpp
src/runner/engine/runnerEngine.cpp 
src/runner/runtime/runtimes/pythonRuntime.cpp
src/runner/runtime/runtimes/defaultRuntime.cpp
src/runner/runtime/runtimeManager.cpp
)


target_include_directories(runner INTERFACE 
src/core/process
src/core/ipc
src/runner
src/runner/engine
src/runner/runtime
src/runner/runtime/runtimes
)

target_link_libraries(runner PRIVATE spdlog::spdlog)

project(script)

include(FetchContent)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
    )
    
    set(JSON_BUILD_STATIC ON CACHE INTERNAL "")
    FetchContent_MakeAvailable(json)
    
    add_executable(script
    src/main.cpp
    src/core/main/scriptManager.cpp
    src/core/main/script.cpp
    src/core/process/process.cpp
    src/core/ipc/namedPipe.cpp
    src/core/ipc/unnamedPipe.cpp
    )
    
    target_include_directories(script INTERFACE 
src/core/main
src/core/main/engine
src/core/process
src/core/ipc
)

target_link_libraries(script PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(script PRIVATE spdlog::spdlog)