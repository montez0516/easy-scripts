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
private:
    std::atomic_bool threadLoop_{true};
    std::function<void(std::string_view)> readyReadCallBack_;
    std::thread readyReadThread_;

protected:
    HANDLE pipeHandle_ = INVALID_HANDLE_VALUE;
    std::string pipeName_;

public:
    ~NamedPipe();

    std::string read() override;
    void write(std::string_view message) override;
    bool readyRead(std::function<void(std::string_view)> readCallBack);
    bool waitForConnection() override;
    void close() override;
    bool isNull();
};
