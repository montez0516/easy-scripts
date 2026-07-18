#include <windows.h>
#include <string>
#include <iostream>

#include "namedPipe.hpp"

#define PIPENAME "\\\\.\\pipe\\easyscripts"

NamedPipe::NamedPipe()
{
    pipeHandle = CreateNamedPipe(TEXT(PIPENAME), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);

    if (pipeHandle == INVALID_HANDLE_VALUE)
    {
        std::cerr << "Failed to create pipe: " << GetLastError() << std::endl;
    }
}

NamedPipe::~NamedPipe()
{
    if (pipeHandle == nullptr)
        return;

    FlushFileBuffers(pipeHandle);
    DisconnectNamedPipe(pipeHandle);
    CloseHandle(pipeHandle);
}

std::string NamedPipe::read()
{
    if (pipeHandle == nullptr)
        return "";
    if (ConnectNamedPipe(pipeHandle, NULL) || GetLastError() == ERROR_PIPE_CONNECTED)
    {
        std::string output;
        char buffer[128];
        DWORD bytesRead = 0;
        if (ReadFile(pipeHandle, buffer, sizeof(buffer) - 1, &bytesRead, NULL))
        {
            output.append(buffer, bytesRead);
            return output;
        }
    }

    return "";
}

void NamedPipe::write(const std::string &data)
{
    if (pipeHandle == nullptr)
        return;

    if (ConnectNamedPipe(pipeHandle, NULL) || GetLastError() == ERROR_PIPE_CONNECTED)
    {
        DWORD bytesToWrite = sizeof(data);
        DWORD bytesWritten = 0;

        WriteFile(pipeHandle, data.data(), bytesToWrite, &bytesWritten, NULL);

        FlushFileBuffers(pipeHandle);
        DisconnectNamedPipe(pipeHandle);
    }
}