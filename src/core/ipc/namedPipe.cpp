#include <windows.h>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include "namedPipe.hpp"

NamedPipe::~NamedPipe()
{
    if (pipeHandle != INVALID_HANDLE_VALUE)
    {
        FlushFileBuffers(pipeHandle);
        DisconnectNamedPipe(pipeHandle);
        CloseHandle(pipeHandle);
    }
}

bool NamedPipe::create(const std::string &name)
{
    pipeHandle = CreateNamedPipe(TEXT(PIPENAME), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);

    if (pipeHandle == INVALID_HANDLE_VALUE)
    {
        spdlog::error("NamedPipe(create): Failed to create pipe. Error: {}\n", GetLastError());
        return false;
    }
    return true;
}

bool NamedPipe::connect(const std::string &name)
{
    pipeHandle = CreateFile(
        TEXT(PIPENAME),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (pipeHandle == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(connect): Failed to connect to pipe. Error: {}\n", GetLastError());
        return false;
    }

    return true;
}

std::string NamedPipe::read()
{
    if (pipeHandle == nullptr || pipeHandle == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(read): pipeHandle is NULL");
        return "";
    }
    std::string output;
    char buffer[512];
    DWORD bytesRead = 0;
    if (!ReadFile(pipeHandle, buffer, sizeof(buffer) - 1, &bytesRead, NULL))
    {
        return "";
    }
    return std::string(buffer, bytesRead);
}

void NamedPipe::write(const std::string &data)
{
    if (pipeHandle == nullptr || pipeHandle == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(write): pipeHandle is NULL");
        return;
    }

    DWORD bytesToWrite = static_cast<DWORD>(data.size());
    DWORD bytesWritten = 0;

    WriteFile(pipeHandle, data.data(), bytesToWrite, &bytesWritten, NULL);
}

bool NamedPipe::isNull()
{
    return pipeHandle == INVALID_HANDLE_VALUE;
}

bool NamedPipe::waitForConnection()
{
    if (pipeHandle == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(waitForConnection): pipeHandle is NULL");
        return false;
    }

    if (ConnectNamedPipe(pipeHandle, nullptr))
    {
        return true;
    }

    DWORD error = GetLastError();

    if (error == ERROR_PIPE_CONNECTED)
    {
        return true;
    }

    spdlog::error("ConnectNamedPipe failed: {}", error);
    return false;
}

nlohmann::json NamedPipe::json()
{
    if (pipeHandle == nullptr || pipeHandle == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(read): pipeHandle is NULL");
        return "";
    }

    char buffer[512];
    DWORD bytesRead = 0;
    if (!ReadFile(pipeHandle, buffer, sizeof(buffer) - 1, &bytesRead, NULL))
    {
        return "";
    }

    return nlohmann::json::parse(buffer);
}