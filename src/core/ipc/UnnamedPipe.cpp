#include "UnnamedPipe.h"
#include "Pipe.h"
#include "../utils/windows/WindowsUtils.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <span>
#include <cstddef>

UnnamedPipe::UnnamedPipe()
{
    SECURITY_ATTRIBUTES saAttr;

    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    if (!CreatePipe(&readHandle_, &writeHandle_, &saAttr, 0))
    {
        spdlog::error("UnnamedPipe(): Fialed to create pipe {}", Utils::Windows::GetError());
        return;
    }
}

UnnamedPipe::~UnnamedPipe()
{
}

HANDLE UnnamedPipe::getReadHandle() const
{
    return readHandle_;
}

HANDLE UnnamedPipe::getWriteHandle() const
{
    return writeHandle_;
}

void UnnamedPipe::closeReadHandle()
{
    if (readHandle_ != nullptr && readHandle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(readHandle_);
        readHandle_ = INVALID_HANDLE_VALUE;
    }
}

void UnnamedPipe::closeWriteHandle()
{
    if (writeHandle_ != nullptr && writeHandle_ != INVALID_HANDLE_VALUE)
    {
        CloseHandle(writeHandle_);
        writeHandle_ = INVALID_HANDLE_VALUE;
    }
}