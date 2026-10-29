#include "Pipe.h"

#include <spdlog/spdlog.h>

#include <windows.h>
#include <span>
#include <cstddef>

ReadResult Pipe::read(std::span<std::byte> buffer)
{
    ReadResult result{};

    HANDLE readHandle = getReadHandle();

    if (readHandle == INVALID_HANDLE_VALUE)
    {
        result.status = READ_ERROR;
        return result;
    }

    BOOL success = ReadFile(
        readHandle,
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

WriteResult Pipe::write(std::span<const std::byte> buffer)
{
    WriteResult result{};

    HANDLE writeHandle = getWriteHandle();

    if (writeHandle == INVALID_HANDLE_VALUE)
    {
        result.status = WRITE_ERROR;
        return result;
    }

    BOOL success = WriteFile(
        writeHandle,
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

bool Pipe::waitForConnection()
{
    return false;
}