#ifndef UNNAMED_PIPE_H
#define UNNAMED_PIPE_H

#include "pipe.hpp"

#include <windows.h>
#include <string>
#include <thread>
#include <functional>
#include <atomic>

class UnnamedPipe : public Pipe
{

private:
    HANDLE readHandle_ = INVALID_HANDLE_VALUE;
    HANDLE writeHandle_ = INVALID_HANDLE_VALUE;

    std::atomic_bool threadLoop_{true};
    std::function<void(std::string_view)> readyReadCallBack_;
    std::thread readyReadThread_;

public:
    UnnamedPipe();
    ~UnnamedPipe();

    std::string read() override;
    void write(std::string_view message) override;
    bool readyRead(std::function<void(std::string_view)> callback) override;
    HANDLE getRead() const;
    HANDLE getWrite() const;

    bool isNull();

    void closeRead();
    void closeWrite();

    void close();
};

#endif