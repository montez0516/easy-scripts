#include "NamedPipeClient.h"
#include "NamedPipe.h"
#include "../utils/windows/WindowsUtils.h"
#include "../utils/Conversion.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <string>

NamedPipeClient::NamedPipeClient(const std::string &pipeName)
{
    pipeHandle_ = CreateFileW(
        Utils::Conversion::toWstring(PIPE_PREFIX + pipeName).c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipeClient(): Failed to connect to pipe. Error: {}\n", Utils::Windows::GetError());
    }
}