#include "NamedPipe.h"

#include <spdlog/spdlog.h>

#include <functional>
#include <thread>
#include <atomic>
#include <windows.h>
#include <string>
#include <string_view>
#include <utility>
#include <cstdint>

NamedPipe::~NamedPipe()
{

    if (pipeHandle_ != INVALID_HANDLE_VALUE)
    {
        FlushFileBuffers(pipeHandle_);
        DisconnectNamedPipe(pipeHandle_);
        closeHandle();
    }
}

HANDLE NamedPipe::getReadHandle() const
{
    return pipeHandle_;
}

HANDLE NamedPipe::getWriteHandle() const
{
    return pipeHandle_;
}

void NamedPipe::closeHandle()
{
    if (pipeHandle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(pipeHandle_);
        pipeHandle_ = INVALID_HANDLE_VALUE;
    }
}

void NamedPipe::closeReadHandle()
{
    closeHandle();
}

void NamedPipe::closeWriteHandle()
{
    closeHandle();
}