#include "namedPipeClient.hpp"
#include "namedPipe.hpp"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <string>
#include <utility>

NamedPipeClient::NamedPipeClient(std::string pipeName)
{
    pipeName_ = std::move(pipeName);
}

bool NamedPipeClient::open()
{
    pipeHandle_ = CreateFile(
        (PIPE_PREFIX + pipeName_).c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipeClient(): Failed to connect to pipe. Error: {}\n", GetLastError());
        return false;
    }

    return true;
}