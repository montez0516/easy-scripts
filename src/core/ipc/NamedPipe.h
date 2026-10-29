#pragma once

#include "Pipe.h"

#include <nlohmann/json.hpp>

#include <windows.h>
#include <string>
#include <functional>
#include <thread>
#include <atomic>

#define PIPE_PREFIX "\\\\.\\pipe\\"

class NamedPipe : public Pipe
{

public:
    ~NamedPipe();

    HANDLE getReadHandle() const override;
    HANDLE getWriteHandle() const override;

    void closeReadHandle() override;
    void closeWriteHandle() override;

protected:
    HANDLE pipeHandle_ = INVALID_HANDLE_VALUE;

private:
    void closeHandle();
};
