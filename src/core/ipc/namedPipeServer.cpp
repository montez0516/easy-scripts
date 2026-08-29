#include "namedPipeServer.hpp"
#include "namedPipe.hpp"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <string>

NamedPipeServer::NamedPipeServer(std::string pipeName)
{
    pipeName_ = pipeName;
}

bool NamedPipeServer::open()
{
    pipeHandle_ = CreateNamedPipe((PIPE_PREFIX + pipeName_).c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);

    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::error("NamedPipeServer(): Failed to create pipe. Error: {}\n", GetLastError());
        return false;
    }
    return true;
}

bool NamedPipeServer::waitForConnection()
{
    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(waitForConnection): pipeHandle is NULL");
        return false;
    }

    if (ConnectNamedPipe(pipeHandle_, nullptr))
    {
        return true;
    }

    DWORD error = GetLastError();

    if (error == ERROR_PIPE_CONNECTED)
    {
        return true;
    }

    spdlog::error("NamedPipe(waitForConnection): ConnectNamedPipe failed: {}", error);
    return false;
}