#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include <nlohmann/json.hpp>
#include <windows.h>
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#define PIPE_PREFIX "\\\\.\\pipe\\"

class NamedPipe
{
private:
    std::atomic_bool threadLoop_{true};
    std::function<void()> readyReadCallBack_;
    std::thread readyReadThread_;

protected:
    HANDLE pipeHandle_ = INVALID_HANDLE_VALUE;
    std::string pipeName_;

public:
    ~NamedPipe();
    virtual bool open() = 0;

    std::string read();
    void write(const std::string &);

    bool isNull();

    bool readyRead(std::function<void()> readCallBack);
};

#endif