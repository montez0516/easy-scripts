#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include <nlohmann/json.hpp>
#include <windows.h>
#include <string>
#include <functional>
#include <thread>

#define PIPENAME "\\\\.\\pipe\\easyscripts"

class NamedPipe
{
private:
    HANDLE pipeHandle_ = INVALID_HANDLE_VALUE;
    std::string pipeName_;

    bool threadLoop_;
    std::function<void()> readyReadCallBack_;
    std::thread readyReadThread_;

public:
    NamedPipe(std::string pipeName = PIPENAME);
    ~NamedPipe();

    bool create();
    bool connect();

    bool waitForConnection();

    std::string read();
    void write(const std::string &);

    bool isNull();

    bool readyRead(std::function<void()> readCallBack);
};

#endif