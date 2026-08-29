#include "namedPipe.hpp"

#include <spdlog/spdlog.h>
#include <nlohmann/json.hpp>

#include <windows.h>
#include <string>
#include <utility>
#include <cstdint>

NamedPipe::~NamedPipe()
{
    threadLoop_.store(false);
    if (readyReadThread_.joinable())
        readyReadThread_.join();

    if (pipeHandle_ != INVALID_HANDLE_VALUE)
    {
        FlushFileBuffers(pipeHandle_);
        DisconnectNamedPipe(pipeHandle_);
        CloseHandle(pipeHandle_);
    }
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
        while(threadLoop_.load())
        {
            DWORD bytesAvail = 0;
            if(PeekNamedPipe(pipeHandle_, NULL, 0, NULL, &bytesAvail, NULL))
            {
                if(bytesAvail > 0)
                {
                    spdlog::debug(
                        "NamedPipe(readyRead): {} bytes available",
                        bytesAvail
                    );

                    readyReadCallBack_();

                    spdlog::debug(
                        "NamedPipe(readyRead): callback returned"
                    );
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