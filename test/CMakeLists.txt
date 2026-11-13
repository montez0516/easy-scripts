cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(test)

include(FetchContent)

FetchContent_Declare(
    spdlog
    GIT_REPOSITORY https://Github.com/gabime/spdlog.git
    GIT_TAG v1.17.0
)

set(SPDLOG_BUILD_STATIC ON CACHE INTERNAL "")
FetchContent_MakeAvailable(spdlog)

FetchContent_Declare(
json
GIT_REPOSITORY https://Github.com/nlohmann/json.git
GIT_TAG v3.12.0
)

set(JSON_BUILD_STATIC ON CACHE INTERNAL "")
FetchContent_MakeAvailable(json)

add_executable(test 
main.cpp
../src/core/parsing/DotEnv.cpp
../src/core/parsing/DotEnvParser.cpp
../src/core/utils/Conversion.cpp
../src/core/utils/windows/WindowsUtils.cpp
)

target_include_directories(test PRIVATE
../src/core/parsing
../src/core/utils/windows
../src/core/utils
)

target_link_libraries(test PRIVATE -static nlohmann_json::nlohmann_json spdlog::spdlog)



