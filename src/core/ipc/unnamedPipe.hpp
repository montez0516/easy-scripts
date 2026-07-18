#ifndef UNNAMED_PIPE_H
#define UNNAMED_PIPE_H

#include <windows.h>
#include <string>

class UnnamedPipe
{

private:
    HANDLE readHandle = nullptr;
    HANDLE writeHandle = nullptr;

public:
    UnnamedPipe();
    ~UnnamedPipe();

    std::string read();
    void write(const std::string &);
    HANDLE getRead() const;
    HANDLE getWrite() const;

    void closeRead();
    void closeWrite();
};

#endif