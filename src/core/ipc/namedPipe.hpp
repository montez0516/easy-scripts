#ifndef NAMED_PIPE_H
#define NAMED_PIPE_H

#include <windows.h>
#include <string>
#include <nlohmann/json.hpp>

#define PIPENAME "\\\\.\\pipe\\easyscripts"

class NamedPipe
{
private:
    HANDLE pipeHandle = INVALID_HANDLE_VALUE;

public:
    ~NamedPipe();

    bool create(const std::string &name = PIPENAME);
    bool connect(const std::string &name = PIPENAME);

    bool waitForConnection();

    std::string read();
    void write(const std::string &);
    nlohmann::json json();

    bool isNull();
};

#endif