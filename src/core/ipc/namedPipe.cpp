#include <windows.h>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>
#include <utility>

#include "namedPipe.hpp"

NamedPipe::NamedPipe(std::string pipeName) : pipeName_(std::move(pipeName)) {}

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
        spdlog::critical("NamedPipe(read): pipeHandle is NULL");
        return "";
    }

    std::size_t payloadSize = 0;

    if (!ReadFile(pipeHandle_, &payloadSize, sizeof(std::size_t), NULL, NULL))
        return {};

    std::string output;
    char buffer[payloadSize];
    DWORD bytesRead = 0;

    if (!ReadFile(pipeHandle_, buffer, sizeof(buffer) - 1, &bytesRead, NULL))
    {
        return "";
    }
    return std::string(buffer, bytesRead);
}

void NamedPipe::write(const std::string &data)
{
    if (pipeHandle_ == nullptr || pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(write): pipeHandle is NULL");
        return;
    }

    DWORD bytesToWrite = static_cast<DWORD>(data.size());
    DWORD bytesWritten = 0;

    WriteFile(pipeHandle_, &bytesToWrite, sizeof(std::size_t), NULL, NULL);

    WriteFile(pipeHandle_, data.data(), bytesToWrite, &bytesWritten, NULL);
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

    spdlog::error("ConnectNamedPipe failed: {}", error);
    return false;
}

nlohmann::json NamedPipe::json()
{
    if (pipeHandle_ == nullptr || pipeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("NamedPipe(read): pipeHandle is NULL");
        return {};
    }

    std::string data = this->read();

    try
    {

        nlohmann::json json = nlohmann::json::parse(data);
        return json;
    }
    catch (nlohmann::json_abi_v3_12_0::detail::parse_error &e)
    {
        spdlog::critical("{}\n{}", e.what(), data);
    }

    return {};
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
                spdlog::error("NamedPipe(readyRead): failed to peek into readHandle");
                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } });

    return true;
}