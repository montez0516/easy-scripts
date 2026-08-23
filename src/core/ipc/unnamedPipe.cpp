#include "unnamedPipe.hpp"
#include "../process/utils.hpp"

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
    threadLoop_ = false;
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

void UnnamedPipe::write(const std::string &data)
{
    if (writeHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("UnnamedPipe(write): writeHandle is null");
        return;
    }

    if (data.empty())
        return;
    DWORD bytesWritten = 0;

    WriteFile(writeHandle_, data.data(), data.size(), &bytesWritten, NULL);
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

bool UnnamedPipe::readyRead(std::function<void()> readCallBack)
{
    if (readHandle_ == INVALID_HANDLE_VALUE)
    {
        spdlog::critical("UnnamedPipe(readyRead): readHandle is invalid");
        return false;
    }
    readyReadCallBack_ = std::move(readCallBack);

    readyReadThread_ = std::thread([this]()
                                   {
        while(threadLoop_)
        {
            DWORD bytesAvail = 0;
            if(PeekNamedPipe(readHandle_, NULL, 0, NULL, &bytesAvail, NULL))
            {
                if(bytesAvail > 0)
                {
                    readyReadCallBack_();
                }
            }
            else{
                spdlog::error("UnnamedPipe(readyRead): failed to peek into readHandle");
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