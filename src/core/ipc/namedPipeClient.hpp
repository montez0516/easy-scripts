#ifndef NAMED_PIPE_CLIENT_H
#define NAMED_PIPE_CLIENT_H

#include "namedPipe.hpp"

#include <string>

class NamedPipeClient : public NamedPipe
{
public:
    NamedPipeClient(std::string pipeName);
    bool open() override;
};

#endif