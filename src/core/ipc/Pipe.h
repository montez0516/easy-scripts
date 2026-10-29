#pragma once

#include <windows.h>
#include <span>
#include <cstddef>

enum ReadStatus
{
    READ_SUCCESS,
    READ_EOF,
    READ_ERROR
};

struct ReadResult
{
    ReadStatus status;
    DWORD bytesRead = 0;
    DWORD errorCode = ERROR_SUCCESS;
};

enum WriteStatus
{
    WRITE_SUCCESS,
    WRITE_ERROR
};

struct WriteResult
{
    WriteStatus status;
    DWORD bytesWritten;
    DWORD errorCode;
};

class Pipe
{
public:
    ReadResult read(std::span<std::byte> buffer);
    WriteResult write(std::span<const std::byte> buffer);

    virtual ~Pipe() = default;

    virtual HANDLE getReadHandle() const = 0;
    virtual HANDLE getWriteHandle() const = 0;

    virtual void closeReadHandle() = 0;
    virtual void closeWriteHandle() = 0;

    virtual bool waitForConnection();
};