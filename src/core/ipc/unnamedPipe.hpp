#ifndef UNNAMED_PIPE_H
#define UNNAMED_PIPE_H

#include <windows.h>
#include <string>
#include <thread>
#include <functional>

class UnnamedPipe
{

private:
    HANDLE readHandle_ = INVALID_HANDLE_VALUE;
    HANDLE writeHandle_ = INVALID_HANDLE_VALUE;

    bool threadLoop_;
    std::function<void()> readyReadCallBack_;
    std::thread readyReadThread_;

public:
    UnnamedPipe();
    ~UnnamedPipe();

    std::string read();
    void write(const std::string &);
    HANDLE getRead() const;
    HANDLE getWrite() const;

    bool isNull();

    void closeRead();
    void closeWrite();

    bool readyRead(std::function<void()> readCallBack);
};

#endif