#include "UnnamedPipe.h"
#include "Pipe.h"
#include "../process/Utils.h"

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
        spdlog::critical("UnnamedPipe(): Fialed to create pipe");
        return;
    }
}

UnnamedPipe::~UnnamedPipe()
{
}

ReadResult UnnamedPipe::read(std::span<std::byte> buffer)
{
    ReadResult result{};

    BOOL success = ReadFile(
        readHandle_,
        buffer.data(),
        static_cast<DWORD>(buffer.size()),
        &result.bytesRead,
        nullptr);

    if (!success)
    {
        DWORD errorCode = GetLastError();
        if (errorCode == ERROR_BROKEN_PIPE)
        {
            result.status = READ_EOF;
        }
        else
        {
            result.status = READ_ERROR;
            result.errorCode = errorCode;
        }

        return result;
    }

    result.status = READ_SUCCESS;
    return result;
}

WriteResult UnnamedPipe::write(std::span<const std::byte> buffer)
{
    WriteResult result{};

    BOOL success = WriteFile(
        writeHandle_,
        buffer.data(),
        static_cast<DWORD>(buffer.size()),
        &result.bytesWritten,
        nullptr);

    if (!success)
    {
        DWORD errorCode = GetLastError();

        result.status = WRITE_ERROR;
        result.errorCode = errorCode;

        return result;
    }

    result.status = WRITE_SUCCESS;
    return result;
}

HANDLE UnnamedPipe::getReadHandle() const
{
    return readHandle_;
}

HANDLE UnnamedPipe::getWriteHandle() const
{
    return writeHandle_;
}
