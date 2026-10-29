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

    HANDLE getReadHandle() const override;
    HANDLE getWriteHandle() const override;

    void closeReadHandle() override;
    void closeWriteHandle() override;
};
