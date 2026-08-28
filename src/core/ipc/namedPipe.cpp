#include "namedPipe.hpp"

#include <windows.h>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <utility>
#include <cstdint>


NamedPipe::NamedPipe(std::string pipeName) {
    pipeName_ = PIPE_PREFIX + pipeName;
}

NamedPipe::~NamedPipe()
{
    if (pipeHandle_ != INVALID_HANDLE_VALUE)
    {
        FlushFileBuffers(pipeHandle_);
        DisconnectNamedPipe(pipeHandle_);
        CloseHandle(pipeHandle_);
    }
}

bool NamedPipe::create()
{
    pipeHandle_ = CreateNamedPipe(pipeName_.c_str(), PIPE_ACCESS_DUPLEX, PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT, PIPE_UNLIMITED_INSTANCES, 512, 512, 0, NULL);

    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::error("NamedPipe(create): Failed to create pipe. Error: {}\n", GetLastError());
        return false;
    }
    return true;
}

bool NamedPipe::connect()
{
    pipeHandle_ = CreateFile(
        pipeName_.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);

    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(connect): Failed to connect to pipe. Error: {}\n", GetLastError());
        return false;
    }

    return true;
}

std::string NamedPipe::read()
{
    if (pipeHandle_ == nullptr || pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(read): pipeHandle is NULL {}", GetLastError());
        return {};
    }

    std::uint32_t payloadSize = 0;

    if (!ReadFile(pipeHandle_, &payloadSize, sizeof(payloadSize), NULL, NULL))
    {
        spdlog::error("NamedPipe(read): failed to read payload size from pipe {}", GetLastError());
        return {};
    }

    std::string output(payloadSize, '\0');
    DWORD bytesRead = 0;

    if (!ReadFile(pipeHandle_, output.data(), payloadSize, &bytesRead, NULL))
    {
        spdlog::error("NamedPipe(read): failed to read payload from pipe {}", GetLastError());
        return {};
    }

    output.resize(bytesRead);
    return output;
}

void NamedPipe::write(const std::string &data)
{
    if (pipeHandle_ == nullptr || pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(write): pipeHandle is NULL {}", GetLastError());
        return;
    }

    std::uint32_t payloadSize = static_cast<std::uint32_t>(data.size());

    if (!WriteFile(pipeHandle_, &payloadSize, sizeof(payloadSize), NULL, NULL))
    {
        spdlog::error("NamedPipe(write): failed to write payload size {}", GetLastError());
        return;
    }

    DWORD bytesWritten = 0;

    if (!WriteFile(pipeHandle_, data.data(), payloadSize, &bytesWritten, NULL))
    {
        spdlog::error("NamedPipe(write): failed to write payload {}", GetLastError());
    }
}

bool NamedPipe::isNull()
{
    return pipeHandle_ == INVALID_HANDLE_VALUE;
}

bool NamedPipe::waitForConnection()
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

bool NamedPipe::readyRead(std::function<void()> readCallBack)
{
    if (pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(readyRead): readHandle is invalid");
        return false;
    }
    readyReadCallBack_ = std::move(readCallBack);

    readyReadThread_ = std::thread([this]()
                                   {
        while(threadLoop_)
        {
            DWORD bytesAvail = 0;
            if(PeekNamedPipe(pipeHandle_, NULL, 0, NULL, &bytesAvail, NULL))
            {
                if(bytesAvail > 0)
                {
                    readyReadCallBack_();
                }
            }
            else{
                spdlog::error("NamedPipe(readyRead): failed to peek into readHandle {}", GetLastError());
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } });

    return true;
}