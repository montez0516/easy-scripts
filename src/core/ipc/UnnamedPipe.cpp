#include "UnnamedPipe.h"
#include "../process/Utils.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <string>
#include <thread>
#include <functional>
#include <chrono>

UnnamedPipe::UnnamedPipe()
{
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&readHandle_, &writeHandle_, &saAttr, 0))
    {
        spdlog::critical("UnnamedPipe(): Fialed to create pipe");
        return;
    }
}

UnnamedPipe::~UnnamedPipe()
{
    threadLoop_.store(false);
    if (readyReadThread_.joinable())
        readyReadThread_.join();
    closeRead();
    closeWrite();
}

std::string UnnamedPipe::read()
{
    if (readHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("UnnamedPipe(read): readHandle is null");
        return "";
    }

    std::string output;

    char buffer[4096];
    DWORD bytesRead = 0;

    if (!ReadFile(readHandle_, buffer, sizeof(buffer), &bytesRead, NULL))
    {
        return "";
    }

    output.append(buffer, bytesRead);

    return output;
}

void UnnamedPipe::write(std::string_view message)
{
    if (writeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("UnnamedPipe(write): writeHandle is invalid");
        return;
    }

    std::string payload{message};

    DWORD bytesWritten = 0;

    if (!WriteFile(
            writeHandle_,
            payload.data(),
            static_cast<DWORD>(payload.size()),
            &bytesWritten,
            nullptr))
    {
        spdlog::error(
            "UnnamedPipe(write): WriteFile failed {}",
            GetLastError());
        return;
    }
}

HANDLE UnnamedPipe::getRead() const
{
    return readHandle_;
}

HANDLE UnnamedPipe::getWrite() const
{
    return writeHandle_;
}

void UnnamedPipe::closeRead()
{
    if (readHandle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(readHandle_);
        readHandle_ = INVALID_HANDLE_VALUE;
    }
}

void UnnamedPipe::closeWrite()
{
    if (writeHandle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(writeHandle_);
        writeHandle_ = INVALID_HANDLE_VALUE;
    }
}

bool UnnamedPipe::readyRead(std::function<void(std::string_view)> readCallBack)
{
    spdlog::debug("UnnamedPipe(readyRead): starting readyRead thread");
    if (readHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("UnnamedPipe(readyRead): readHandle is invalid");
        return false;
    }
    readyReadCallBack_ = std::move(readCallBack);

    readyReadThread_ = std::thread([this]()
                                   {
        while(threadLoop_.load())
        {
            DWORD bytesAvail = 0;
            if(PeekNamedPipe(readHandle_, NULL, 0, NULL, &bytesAvail, NULL))
            {
                if(bytesAvail > 0)
                {
                    readyReadCallBack_(read());
                }
            }
            else{
                DWORD code = GetLastError();

                if(code != ERROR_BROKEN_PIPE)
                {
                    spdlog::error("UnnamedPipe(readyRead): failed to peek into readHandle. {}", code);
                }

                return;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } });

    return true;
}

bool UnnamedPipe::isNull()
{
    return (readHandle_ == INVALID_HANDLE_VALUE || writeHandle_ == INVALID_HANDLE_VALUE);
}

void UnnamedPipe::close()
{
    threadLoop_.store(false);
    if (readyReadThread_.joinable())
        readyReadThread_.join();

    if (readHandle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(readHandle_);
        readHandle_ = INVALID_HANDLE_VALUE;
    }

    if (writeHandle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(writeHandle_);
        writeHandle_ = INVALID_HANDLE_VALUE;
    }
}