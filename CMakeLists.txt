cmake_minimum_required(VERSION 4.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(EasyScripts)

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

set(CMAKE_AUTOMOC ON)
set(Qt6_DIR "C:\\Qt\\6.11.2\\mingw_64\\lib\\cmake\\Qt6")

set(CMAKE_FIND_LIBRARY_PREFIXES ".a")
set(BUILD_SHARED_LIBS OFF)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

add_executable(EasyScripts
src/main.cpp

src/core/Paths.cpp
src/core/utils/Conversion.cpp
src/core/utils/windows/WindowsUtils.cpp
src/core/utils/windows/Environment.cpp
src/core/process/ProcessHandler.cpp
src/core/ipc/Pipe.cpp
src/core/ipc/NamedPipe.cpp
src/core/ipc/UnnamedPipe.cpp
src/core/ipc/NamedPipeServer.cpp
src/core/ipc/NamedPipeClient.cpp
src/core/communication/IPCConnection.cpp
src/core/communication/MessageCodec.cpp
src/core/communication/MessageFramer.cpp
src/core/eventBus/EventBus.cpp

src/main/ScriptManager.cpp
src/main/ScriptInfo.cpp
src/main/apis/FileSystemAPI.cpp
src/main/apis/MediaAPI.cpp

src/api/Register.cpp

src/ui/MainWindow.cpp
src/ui/utils/ThemeManager.cpp
src/ui/utils/ThemeManager.h
src/ui/CentralWidget.h
src/ui/CentralWidget.cpp
src/ui/reuseables/GridWidget.h
src/ui/reuseables/GridWidget.cpp
src/ui/reuseables/NavBar.h
src/ui/reuseables/NavBar.cpp
src/ui/reuseables/IconButton.h
src/ui/reuseables/IconButton.cpp
src/ui/reuseables/Image.h
src/ui/reuseables/Image.cpp
src/ui/reuseables/FlowLayout.h
src/ui/reuseables/FlowLayout.cpp
src/ui/main/ScriptCard.h
src/ui/main/ScriptCard.cpp
src/ui/main/ScriptPage.h
src/ui/main/CreatePage.h
src/ui/main/CreatePage.cpp
src/ui/UIContext.h
src/ui/main/ScriptPage.cpp
src/ui/main/DetailsPage.h
src/ui/main/DetailsPage.cpp
src/ui/utils/IconManager.h
src/ui/utils/IconManager.cpp
)

add_executable(runner
src/runner.cpp

src/core/Paths.cpp
src/core/utils/Conversion.cpp
src/core/utils/windows/WindowsUtils.cpp
src/core/utils/windows/Environment.cpp
src/core/process/ProcessHandler.cpp
src/core/ipc/Pipe.cpp
src/core/ipc/NamedPipe.cpp
src/core/ipc/UnnamedPipe.cpp
src/core/ipc/NamedPipeServer.cpp
src/core/ipc/NamedPipeClient.cpp
src/core/communication/IPCConnection.cpp
src/core/communication/MessageCodec.cpp
src/core/communication/MessageFramer.cpp
src/core/eventBus/EventBus.cpp

src/runner/engine/RunnerEngine.cpp 
src/runner/runtime/Runtimes/PythonRuntime.cpp
src/runner/runtime/Runtimes/DefaultRuntime.cpp
src/runner/runtime/RuntimeManager.cpp
src/runner/runtime/Runtime.cpp
)

target_include_directories(EasyScripts PRIVATE 
src/main
src/core
src/core/process
src/core/utils
src/core/utils/windows

src/core/ipc
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
src/core/utils
src/core/utils/windows
src/core/process
src/core/ipc
src/core/eventBus
src/core/communication
src/runner
src/runner/engine
src/runner/runtime
src/runner/runtime/runtimes
)

target_link_libraries(EasyScripts PRIVATE -static nlohmann_json::nlohmann_json spdlog::spdlog Qt6::Widgets Qt6::Core)
target_link_libraries(runner PRIVATE -static nlohmann_json::nlohmann_json spdlog::spdlog)

target_compile_definitions(EasyScripts PRIVATE 
    $<$<CONFIG:Debug>:BUILD_DEV=1>
)

target_compile_definitions(runner PRIVATE 
    $<$<CONFIG:Debug>:BUILD_DEV=1>
)



install(TARGETS EasyScripts
RUNTIME DESTINATION .
)

install(DIRECTORY resources DESTINATION .)
install(DIRECTORY sdk DESTINATION .)

install(TARGETS runner RUNTIME DESTINATION bin)
