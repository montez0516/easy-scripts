#include "NamedPipeServer.h"
#include "NamedPipe.h"
#include "../utils/windows/WindowsUtils.h"
#include "../utils/Conversion.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <string>

NamedPipeServer::NamedPipeServer(const std::string &pipeName)
{
    pipeHandle_ = CreateNamedPipeW(Utils::Conversion::toWstring(PIPE_PREFIX + pipeName).c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);

    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::error("NamedPipeServer(): Failed to create pipe. Error: {}\n", Utils::Windows::GetError());
    }
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

    spdlog::error("NamedPipe(waitForConnection): ConnectNamedPipe failed: {}", Utils::Windows::GetErrorFromCode(error));
    return false;
}