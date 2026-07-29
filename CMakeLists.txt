cmake_minimum_required(VERSION 4.0.0)


project(EasyScripts)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

set(SPDLOG_BUILD_STATIC ON CACHE INTERNAL "")
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
json
GIT_REPOSITORY https://github.com/nlohmann/json.git
GIT_TAG v3.12.0
)

set(JSON_BUILD_STATIC ON CACHE INTERNAL "")
FetchContent_MakeAvailable(json)

add_executable(EasyScripts
src/main.cpp
src/core/paths.cpp
src/main/scriptManager.cpp
src/main/script.cpp
src/core/process/process.cpp
src/core/ipc/namedPipe.cpp
src/core/ipc/unnamedPipe.cpp
)

add_executable(runner
src/runner.cpp
src/core/paths.cpp
src/core/process/process.cpp
src/core/ipc/namedPipe.cpp
src/core/ipc/unnamedPipe.cpp
src/runner/engine/runnerEngine.cpp 
src/runner/runtime/runtimes/pythonRuntime.cpp
src/runner/runtime/runtimes/defaultRuntime.cpp
src/runner/runtime/runtimeManager.cpp
)

target_include_directories(EasyScripts INTERFACE 
src/main
src/core/process
src/core/ipc
src/core
)

target_include_directories(runner INTERFACE 
src/core
src/core/process
src/core/ipc
src/runner
src/runner/engine
src/runner/runtime
src/runner/runtime/runtimes
)

target_link_libraries(EasyScripts PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog)
target_link_libraries(runner PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog)



install(TARGETS EasyScripts
RUNTIME DESTINATION .
)

install(TARGETS runner
    RUNTIME DESTINATION bin
)

install(DIRECTORY scripts/
    DESTINATION scripts
)