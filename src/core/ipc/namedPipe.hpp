#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include <windows.h>
#include <string>

class NamedPipe
{
private:
    HANDLE pipeHandle = nullptr;

public:
    NamedPipe();
    ~NamedPipe();

    std::string read();
    void write(const std::string &);
};

#endif