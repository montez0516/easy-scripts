cmake_minimum_required(VERSION 4.0.0)


# project(runner)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

set(SPDLOG_BUILD_STATIC ON CACHE INTERNAL "")
FetchContent_MakeAvailable(spdlog)

# add_executable(runner
# runner.cpp 
# core/process/process.cpp
# core/runner/engine/runnerEngine.cpp 
# core/runner/runtime/runtimes/pythonRuntime.cpp
# core/runner/runtime/runtimes/defaultRuntime.cpp
# core/runner/runtime/runtimeManager.cpp
# core/ipc/namedPipe.cpp
# core/ipc/unnamedPipe.cpp
# )


# target_include_directories(runner INTERFACE core/process core/runner core/runner/engine core/runner/runtime core/runner/runtime/runtimes core/ipc)
# target_link_libraries(runner PRIVATE spdlog::spdlog)

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
)

target_include_directories(script INTERFACE src/core/main)
target_link_libraries(script PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(script PRIVATE spdlog::spdlog)