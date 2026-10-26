#pragma once

#include "Pipe.h"

#include <windows.h>
#include <span>
#include <cstddef>

class UnnamedPipe : public Pipe
{

private:
    HANDLE readHandle_ = INVALID_HANDLE_VALUE;
    HANDLE writeHandle_ = INVALID_HANDLE_VALUE;

public:
    UnnamedPipe();
    ~UnnamedPipe();

    ReadResult read(std::span<std::byte> buffer) override;
    WriteResult write(std::span<const std::byte> buffer) override;

    HANDLE getReadHandle() const override;
    HANDLE getWriteHandle() const override;
};
