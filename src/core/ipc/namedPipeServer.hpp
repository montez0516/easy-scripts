#ifndef NAMED_PIPE_SERVER_H
#define NAMED_PIPE_SERVER_H

#include "namedPipe.hpp"

#include <string>

class NamedPipeServer : public NamedPipe
{
public:
    NamedPipeServer(std::string pipeName);
    bool open() override;
    bool waitForConnection();
};

#endif