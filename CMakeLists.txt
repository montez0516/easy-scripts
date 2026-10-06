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

set(CMAKE_AUTOMOC ON)
set(Qt6_DIR "C:\\Qt\\6.11.2\\mingw_64\\lib\\cmake\\Qt6")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(EasyScripts
src/main.cpp

src/core/paths.cpp
src/core/process/process.cpp
src/core/ipc/pipe.cpp
src/core/ipc/namedPipe.cpp
src/core/ipc/unnamedPipe.cpp
src/core/ipc/namedPipeServer.cpp
src/core/ipc/namedPipeClient.cpp
src/core/communication/ipcConnection.cpp
src/core/communication/messageCodec.cpp
src/core/communication/messageFramer.cpp


src/main/scriptManager.cpp
src/main/apis/fileAPI.cpp
src/main/apis/mediaAPI.cpp
src/main/scriptInfo.cpp

src/api/register.cpp

src/ui/mainWindow.cpp
src/ui/themeManager.cpp
src/ui/themeManager.hpp
src/ui/centralWidget.hpp
src/ui/centralWidget.cpp
src/ui/reuseables/gridWidget.hpp
src/ui/reuseables/gridWIdget.cpp
src/ui/main/scriptCard.hpp
src/ui/main/scriptCard.cpp
src/ui/reuseables/navBar.hpp
src/ui/reuseables/navBar.cpp
src/ui/reuseables/iconButton.hpp
src/ui/reuseables/iconButton.cpp
src/ui/reuseables/image.hpp
src/ui/reuseables/image.cpp
)

add_executable(runner
src/runner.cpp

src/core/paths.cpp
src/core/process/process.cpp
src/core/ipc/pipe.cpp
src/core/ipc/namedPipe.cpp
src/core/ipc/unnamedPipe.cpp
src/core/ipc/namedPipeServer.cpp
src/core/ipc/namedPipeClient.cpp
src/core/communication/ipcConnection.cpp
src/core/communication/messageCodec.cpp
src/core/communication/messageFramer.cpp
src/core/eventBus/eventBus.cpp

src/runner/engine/runnerEngine.cpp 
src/runner/runtime/runtimes/pythonRuntime.cpp
src/runner/runtime/runtimes/defaultRuntime.cpp
src/runner/runtime/runtimeManager.cpp
src/runner/runtime/runtime.cpp
)

target_include_directories(EasyScripts PRIVATE 
src/main
src/core/process
src/core/ipc
src/core
src/core/eventBus
src/core/communication
src/api
src/main/apis
src/ui
src/ui/reuseables
src/ui/main
)

target_include_directories(runner PRIVATE 
src/core
src/core/process
src/core/ipc
src/core/eventBus
src/core/communication
src/runner
src/runner/engine
src/runner/runtime
src/runner/runtime/runtimes
)

target_link_libraries(EasyScripts PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog Qt6::Widgets Qt6::Core)
target_link_libraries(runner PRIVATE nlohmann_json::nlohmann_json spdlog::spdlog)

target_compile_definitions(EasyScripts PRIVATE 
    $<$<CONFIG:Debug>:BUILD_DEV=1>
)

target_compile_definitions(runner PRIVATE 
    $<$<CONFIG:Debug>:BUILD_DEV=1>
)



install(TARGETS EasyScripts
RUNTIME DESTINATION .
)

install(TARGETS runner
    RUNTIME DESTINATION bin
)
